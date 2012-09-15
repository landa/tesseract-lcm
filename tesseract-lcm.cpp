#include <lcm/lcm-cpp.hpp>
#include "bot_core/image_t.hpp"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <iostream>
#include <stdio.h>

using namespace std;

class Handler {
  public:
    ~Handler() {}

    void handleMessage(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan, 
                       const bot_core::image_t* msg) {
      printf("Received message on channel \"%s\":\n", chan.c_str());

      long width = msg->width;
      long height = msg->height;
      long row_stride = msg->row_stride;
      long bytes_per_pixel = msg->size/(width*height);
      unsigned char image_data[msg->size];
      copy(msg->data.begin(), msg->data.end(), image_data);

      tesseract::TessBaseAPI *tess = new tesseract::TessBaseAPI();

      if (tess->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
      }

      tess->SetImage((unsigned char*) image_data, width, height,
        bytes_per_pixel, row_stride);

      const char* out = tess->GetUTF8Text();

      cout << out << endl;

      tess->Clear();
      tess->End();
      delete [] out;
    }
};


int main(int argc, char** argv) {
  lcm::LCM lcm;

  if (argc <= 1) {
    cout << "Usage: tesseract_lcm channel_name" << endl;
    exit(0);
  }
  char* channel = argv[1];

  if(!lcm.good()) return 1;

  Handler handlerObject;
  lcm.subscribe(channel, &Handler::handleMessage, &handlerObject);

  while(0 == lcm.handle());

  return 0;
}
