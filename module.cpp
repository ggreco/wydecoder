#include <napi.h>
#include "simple_decoder.h"
#include "reverse_decoder.h"
#include <string>
#include "logger.h"

extern "C" {
#include <libavformat/avformat.h>
}

static VideoDecoder *decoder = nullptr;
static std::string filename;
static std::string direction = "none";

Napi::Boolean videoLoad(const Napi::CallbackInfo &info) {
  bool rc = false;
  if (info.Length() > 0 && info[0].IsString()) {
    filename = info[0].ToString();

    try {
      if (decoder)
        delete decoder;

      decoder = new SimpleDecoder(filename);
      direction = "forward";
      rc = true;
    } catch(std::string &err) {
      ELOG << "Unable to open " << filename;
    } 
  }
  else
      WLOG << "Wrong argument type";

  return Napi::Boolean::New(info.Env(), rc);
}

Napi::Boolean setDirection(const Napi::CallbackInfo &info) {
  bool rc = false;
  if (info.Length() > 0 && info[0].IsString() && decoder) {
    std::string wanted = info[0].ToString();
    if ( (wanted == "forward" && dynamic_cast<SimpleDecoder *>(decoder)) ||
         (wanted == "backward" && dynamic_cast<ReverseDecoder *>(decoder)) )
         rc = true;
    else {
      try {
        double pts = decoder->next_pts();
        
        if (wanted == "forward") {
          if (pts < 0) pts = 0;
          delete decoder;
          decoder = new SimpleDecoder(filename, false, pts);
        } else {
          if (pts < 0) pts = decoder->total_length();
          delete decoder;
          decoder = new ReverseDecoder(filename, pts);
        }
        direction = wanted;
        rc = true;
      } catch(std::string &err) {
        ELOG << "Unable to open " << filename << " for " << wanted << " playback";
      } 
    }
  }
  return Napi::Boolean::New(info.Env(), rc);
}

Napi::String getDirection(const Napi::CallbackInfo &info) {
  return Napi::String::New(info.Env(), direction);
}

Napi::Boolean startDecoder(const Napi::CallbackInfo &info) {
  bool rc = false;
  if (decoder)
    rc = decoder->start();

  return Napi::Boolean::New(info.Env(), rc);
}

Napi::Object getFrame(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Object obj = Napi::Object::New(env);
  int timeout = -1;

  if (info.Length() > 0 && info[0].IsNumber())
    timeout = info[0].ToNumber();

  if (!decoder)
    return obj;

  static std::shared_ptr<AVFrame> frame;
  if (!decoder->frames().dequeue(frame, timeout))
    return obj;

  obj.Set(Napi::String::New(env, "width"), Napi::Number::New(env, frame->width));
  obj.Set(Napi::String::New(env, "height"), Napi::Number::New(env, frame->height));
  obj.Set(Napi::String::New(env, "pts"), Napi::Number::New(env, decoder->frame_time(frame->pts)));
  obj.Set(Napi::String::New(env, "lum_data"), Napi::Buffer<uint8_t>::New(env, frame->data[0], frame->linesize[0] * frame->height));
  obj.Set(Napi::String::New(env, "u_data"), Napi::Buffer<uint8_t>::New(env, frame->data[1], frame->linesize[1] * frame->height / 2));
  obj.Set(Napi::String::New(env, "v_data"), Napi::Buffer<uint8_t>::New(env, frame->data[2], frame->linesize[2] * frame->height / 2));

  /*  Napi::ArrayBuffer::New(env, frame->data[0], frame->linesize[0] * frame->height + frame->linesize[1] * frame->height) */

  return obj;
}

Napi::Boolean discardFrame(const Napi::CallbackInfo &info) {
  bool rc = false;
  if (decoder) {
    decoder->frames().pop();
    rc = true;
  }

  return Napi::Boolean::New(info.Env(), rc);
}

Napi::Boolean videoEOF(const Napi::CallbackInfo &info) {
  bool rc = false;
  if (decoder)
    rc = decoder->eof();

  return Napi::Boolean::New(info.Env(), rc);
}

Napi::Boolean seekVideo(const Napi::CallbackInfo &info) {
  bool rc = false;
  if (info[0].IsNumber() && decoder) {
    rc = true;
    double value = info[0].ToNumber();
    ILOG << "Seeking to " << value;
    decoder->seek(value);
  }
  return Napi::Boolean::New(info.Env(), rc);
}

Napi::Number videoLength(const Napi::CallbackInfo &info) {
  double length = -1;
  if (decoder)
    length = decoder->total_length();

  return Napi::Number::New(info.Env(), length);
}

Napi::Number nextPts(const Napi::CallbackInfo &info) {
  double pts = -1;
  if (decoder)
    pts = decoder->next_pts();

  return Napi::Number::New(info.Env(), pts);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Logger::Instance().dest(Logger::LogToStderr);
  ILOG << "Module intialized";
  exports.Set(
    Napi::String::New(env, "load"),
    Napi::Function::New(env, videoLoad)
  );
  exports.Set(
    Napi::String::New(env, "start"),
    Napi::Function::New(env, startDecoder)
  );
  exports.Set(
    Napi::String::New(env, "frame"),
    Napi::Function::New(env, getFrame)
  );
  exports.Set(
    Napi::String::New(env, "eof"),
    Napi::Function::New(env, videoEOF)
  );
  exports.Set(
    Napi::String::New(env, "seek"),
    Napi::Function::New(env, seekVideo)
  );
  exports.Set(
    Napi::String::New(env, "next"),
    Napi::Function::New(env, nextPts)
  );
  exports.Set(
    Napi::String::New(env, "length"),
    Napi::Function::New(env, videoLength)
  );
  exports.Set(
    Napi::String::New(env, "set_direction"),
    Napi::Function::New(env, setDirection)
  );
  exports.Set(
    Napi::String::New(env, "get_direction"),
    Napi::Function::New(env, getDirection)
  );
  exports.Set(
    Napi::String::New(env, "discard"),
    Napi::Function::New(env, discardFrame)
  );
  return exports;
}

// register module
NODE_API_MODULE(wydecoder, Init)