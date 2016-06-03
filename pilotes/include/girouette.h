#ifndef _GIROUETTE_H_
#define _GIROUETTE_H_

void initGirouette(void);
void Config_EXTI_Girouette(void);
void EXTI9_5_GirouetteHandler(void);
float GetAngle(void);
void UpdateGrandeVoile(void);
float absFloat(float input);

#endif
