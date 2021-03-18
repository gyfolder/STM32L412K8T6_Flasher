#ifndef _SBGPU_H_
#define _SBGPU_H_

#ifdef __cplusplus
extern "C"
{
#endif

void Flasher_Init();
void Flasher_Loop();
void I2C_Init();

#ifdef __cplusplus
}
#endif

#endif//_SBGPU_H_