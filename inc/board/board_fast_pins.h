/*
 * pins_fast.h
 *
 *  Created on: Aug 22, 2016
 *      Author: iceman
 */

#ifndef PINS_FAST_H_
#define PINS_FAST_H_


// http://stackoverflow.com/a/1597129
#define VARIABLE_NAME_CONCAT(x, y) x ## y
#define VARIABLE_NAME(x, y) VARIABLE_NAME_CONCAT(x, y)

#define FAST_ZPIN_HANDLE(handleName) \
		volatile uint32_t *VARIABLE_NAME(handleName, PinSetPort); \
		volatile uint32_t *VARIABLE_NAME(handleName, PinClrPort); \
		volatile uint32_t *VARIABLE_NAME(handleName, PinTogglePort); \
		uint32_t VARIABLE_NAME(handleName, PinPosition);

#define FAST_ZPIN_INIT(handleName, arduinoPin) \
		Board_Fast_Pins_Handle_Init( \
			&(VARIABLE_NAME(handleName, PinSetPort)), \
			&(VARIABLE_NAME(handleName, PinClrPort)), \
			&(VARIABLE_NAME(handleName, PinTogglePort)), \
			&(VARIABLE_NAME(handleName, PinPosition)), \
			arduinoPin); \

#define FAST_ZPIN_HIGH(handleName) *(VARIABLE_NAME(handleName, PinSetPort)) = VARIABLE_NAME(handleName, PinPosition);
#define FAST_ZPIN_LOW(handleName) *(VARIABLE_NAME(handleName, PinClrPort)) = VARIABLE_NAME(handleName, PinPosition);
#define FAST_ZPIN_TOGGLE(handleName) *(VARIABLE_NAME(handleName, PinTogglePort)) = VARIABLE_NAME(handleName, PinPosition);

#ifdef __cplusplus
extern "C" {
#endif

extern void Board_Fast_Pins_Handle_Init(
		volatile uint32_t **pinSetHandle,
		volatile uint32_t **pinClrHandle,
		volatile uint32_t **pinToggleHandle,
		volatile uint32_t *pinPositionHandle,
		int arduinoPin);

#ifdef __cplusplus
}
#endif

#endif /* PINS_FAST_H_ */
