#ifndef MOTOR_MOVEMENT_H
#define MOTOR_MOVEMENT_H

#include <Arduino.h>

#define dirPin 13
#define stepPin 15

struct CurrentSet {
    int direction;
    int stepsPerRevolution;

    CurrentSet(int direction, int stepsPerRevolution) {
        this->direction = direction;
        this->stepsPerRevolution = stepsPerRevolution;
    }

    void setStats(int direction, int stepsPerRevolution) {
        this->direction = direction;
        this->stepsPerRevolution = stepsPerRevolution;
    }

    void setMotorStats(int direction) {
        if (direction != this->direction)
        {
            this->stepsPerRevolution = this->stepsPerRevolution / 2;
            this->direction = direction;
        }
    }
};

void motor_move_up(int stepsPerRevolution, int movementSpeed);

void motor_move_down(int stepsPerRevolution, int movementSpeed);

#endif