#include "Motor_movement.h"

void motor_move_up(int stepsPerRevolution, int movementSpeed) {
    // zmieniamy kierunek obrotu silnika
	digitalWrite(dirPin, LOW);

	// obracamy szybko
	for(int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(movementSpeed);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(movementSpeed);
	}
}

void motor_move_down(int stepsPerRevolution, int movementSpeed) {
    // ustalamy kierunek obrotu zgodny z kierunkiem wskazówek zegara
	digitalWrite(dirPin, HIGH);

	// wolno obracamy silnikiem
	for(int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(movementSpeed);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(movementSpeed);
	}
	//delay(500); // czekamy sekundę
}