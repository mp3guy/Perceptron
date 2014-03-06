/*
 * Perceptron.h
 *
 *  Created on: 21 Sep 2010
 *      Author: thomas
 */

#ifndef PERCEPTRON_H_
#define PERCEPTRON_H_

#include <string.h>

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits>
#include <iostream>

using namespace std;

#define IMAGE_SIZE 200

IplImage * drawSpace;
IplImage * weightSpace;

void init();
void getInputX();
int calcF();

bool initialised = false;

int inputVectorX[IMAGE_SIZE * IMAGE_SIZE + 1];
double weightVectorW[IMAGE_SIZE * IMAGE_SIZE + 1];

double learningRateMu = 0.01;

vector<bool> rate;

CvFont font;

int iteration = 0;

#endif /* PERCEPTRON_H_ */
