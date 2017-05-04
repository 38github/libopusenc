/* Copyright (c) 2017 Jean-Marc Valin */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#if !defined(_opusenc_h)
# define _opusenc_h (1)


# if defined(__cplusplus)
extern "C" {
# endif

#include "opus.h"

#ifndef OPE_EXPORT
# if defined(WIN32)
#  if defined(OPE_BUILD) && defined(DLL_EXPORT)
#   define OPE_EXPORT __declspec(dllexport)
#  else
#   define OPE_EXPORT
#  endif
# elif defined(__GNUC__) && defined(OPE_BUILD)
#  define OPE_EXPORT __attribute__ ((visibility ("default")))
# else
#  define OPE_EXPORT
# endif
#endif

#define OPE_OK 0
/* Based on the relevant libopus code minus 10. */
#define OPE_BAD_ARG -11
#define OPE_INTERNAL_ERROR -13
#define OPE_UNIMPLEMENTED -15
#define OPE_ALLOC_FAIL -17

/* Specific to libopusenc. */
#define OPE_CANNOT_OPEN -30
#define OPE_TOO_LATE -31


/* These are the "raw" request values -- they should usually not be used. */
#define OPE_SET_DECISION_DELAY_REQUEST 14000
#define OPE_GET_DECISION_DELAY_REQUEST 14001
#define OPE_SET_MUXING_DELAY_REQUEST 14002
#define OPE_GET_MUXING_DELAY_REQUEST 14003
#define OPE_SET_COMMENT_PADDING_REQUEST 14004
#define OPE_GET_COMMENT_PADDING_REQUEST 14005
#define OPE_SET_SERIALNO_REQUEST 14006
#define OPE_GET_SERIALNO_REQUEST 14007

#define OPE_SET_DECISION_DELAY(x) OPE_SET_DECISION_DELAY_REQUEST, __opus_check_int(x)
#define OPE_GET_DECISION_DELAY(x) OPE_GET_DECISION_DELAY_REQUEST, __opus_check_int_ptr(x)
#define OPE_SET_MUXING_DELAY(x) OPE_SET_MUXING_DELAY_REQUEST, __opus_check_int(x)
#define OPE_GET_MUXING_DELAY(x) OPE_GET_MUXING_DELAY_REQUEST, __opus_check_int_ptr(x)
#define OPE_SET_COMMENT_PADDING(x) OPE_SET_COMMENT_PADDING_REQUEST, __opus_check_int(x)
#define OPE_GET_COMMENT_PADDING(x) OPE_GET_COMMENT_PADDING_REQUEST, __opus_check_int_ptr(x)
#define OPE_SET_SERIALNO(x) OPE_SET_SERIALNO_REQUEST, __opus_check_int(x)
#define OPE_GET_SERIALNO(x) OPE_GET_SERIALNO_REQUEST, __opus_check_int_ptr(x)


typedef int (*ope_write_func)(void *user_data, const unsigned char *ptr, int len);

typedef int (*ope_close_func)(void *user_data);

/** Callback functions for accessing the stream. */
typedef struct {
  /** Callback for writing to the stream. */
  ope_write_func write;
  /** Callback for closing the stream. */
  ope_close_func close;
} OpusEncCallbacks;

/** Opaque encoder struct. */
typedef struct OggOpusEnc OggOpusEnc;

/** Create a new OggOpus file. */
OPE_EXPORT OggOpusEnc *ope_create_file(const char *path, int rate, int channels, int family, int *error);

/** Create a new OggOpus file (callback-based). */
OPE_EXPORT OggOpusEnc *ope_create_callbacks(const OpusEncCallbacks *callbacks, void *user_data,
    int rate, int channels, int family, int *error);

/** Add/encode any number of float samples to the file. */
OPE_EXPORT int ope_write_float(OggOpusEnc *enc, const float *pcm, int samples_per_channel);

/** Add/encode any number of int16 samples to the file. */
OPE_EXPORT int ope_write(OggOpusEnc *enc, const opus_int16 *pcm, int samples_per_channel);

/** Close/finalize the stream. */
OPE_EXPORT int ope_close_and_free(OggOpusEnc *enc);

/** Ends the stream and create a new stream within the same file. */
OPE_EXPORT int ope_chain_current(OggOpusEnc *enc);

/** Ends the stream and create a new file. */
OPE_EXPORT int ope_continue_new_file(OggOpusEnc *enc, const char *path);

/** Ends the stream and create a new file (callback-based). */
OPE_EXPORT int ope_continue_new_callbacks(OggOpusEnc *enc, void *user_data);

/** Add a comment to the file (can only be called before encoding samples). */
OPE_EXPORT int ope_add_comment(OggOpusEnc *enc, const char *tag, const char *val);

/** Add a picture to the file (can only be called before encoding samples). */
OPE_EXPORT int ope_add_picture(OggOpusEnc *enc, const char *spec);

/** Sets the Opus comment vendor string (optional, defaults to library info). */
OPE_EXPORT int ope_set_vendor_string(OggOpusEnc *enc, const char *vendor);

/** Write out the header now rather than wait for audio to begin. */
OPE_EXPORT int ope_flush_header(OggOpusEnc *enc);

/** Goes straight to the libopus ctl() functions. */
OPE_EXPORT int ope_encoder_ctl(OggOpusEnc *enc, int request, ...);

/** ctl()-type call for the OggOpus layer. */
OPE_EXPORT int ope_set_params(OggOpusEnc *enc, int request, ...);

# if defined(__cplusplus)
}
# endif

#endif
