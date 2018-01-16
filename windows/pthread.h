#ifndef USBDMX_PTHREAD_H
#define USBDMX_PTHREAD_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* pthread_t;
void pthread_create(pthread_t * thread_pointer, void * null, void * (*function) (void*), void * pointer);

#ifdef __cplusplus
}
#endif

#endif // USBDMX_PTHREAD_H
