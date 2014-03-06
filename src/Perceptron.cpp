/*
 * Perceptron.cpp
 *
 *  Created on: 21 Sep 2010
 *      Author: thomas
 */

#include "Perceptron.h"

int main()
{
	init();

	while(true)
	{
		char key = cvWaitKey(0);

		if(key == 'a')
		{
			if(!initialised)
			{
				for(int i = 0; i < IMAGE_SIZE * IMAGE_SIZE + 1; i++)
				{
					weightVectorW[i] = (double)rand()/(double)RAND_MAX;
				}

				weightVectorW[IMAGE_SIZE * IMAGE_SIZE] = 0;
			}

			while(true)
			{
				usleep(100000);
				memset(drawSpace->imageData, 0, drawSpace->height *  drawSpace->width * drawSpace->nChannels);
				int decision = rand() % 2;

				//TL to BR
				if(decision == 1)
				{
					cvLine(drawSpace, cvPoint(10 + pow(-1, rand() % 2) * (rand() % 11), 10 + pow(-1, rand() % 2) * (rand() % 11)), cvPoint(drawSpace->width - 12 + pow(-1, rand() % 2) * (rand() % 11), drawSpace->height - 12 + pow(-1, rand() % 2) * (rand() % 11)), CV_RGB(255, 255, 255), 2, 1, 0);
					cvLine(drawSpace, cvPoint(drawSpace->width - 12 + pow(-1, rand() % 2) * (rand() % 11), 10 + pow(-1, rand() % 2) * (rand() % 11)), cvPoint(10 + pow(-1, rand() % 2) * (rand() % 11), drawSpace->height - 12 + pow(-1, rand() % 2) * (rand() % 11)), CV_RGB(255, 255, 255), 2, 1, 0);
				}
				else
				{
					int shift = 0 + rand() % drawSpace->width;
					cvLine(drawSpace, cvPoint(shift, 5 + pow(-1, rand() % 2) * (rand() % 6)), cvPoint(shift, drawSpace->height - 6 + pow(-1, rand() % 2) * (rand() % 6)), CV_RGB(255, 255, 255), 2, 1, 0);
					cvLine(drawSpace, cvPoint(5 + pow(-1, rand() % 2) * (rand() % 6), shift), cvPoint(drawSpace->width - 6 + pow(-1, rand() % 2) * (rand() % 6), shift), CV_RGB(255, 255, 255), 2, 1, 0);
				}

				getInputX();
				int y = calcF();
				int delta = 0;

				initialised = true;

				bool skip = false;

				if(decision == 1)
				{
					if(y == -1)
					{
						delta = y - 1;
						rate.push_back(false);
					}
					else
					{
						rate.push_back(true);
						skip = true;
					}
				}
				else
				{
					if(y == 1)
					{
						delta = y + 1;
						rate.push_back(false);
					}
					else
					{
						rate.push_back(true);
						skip = true;
					}
				}

				if(!skip)
				{
					double maxWeight = numeric_limits<double>::min();
					double minWeight = numeric_limits<double>::max();

					for(int i = 0; i <= IMAGE_SIZE * IMAGE_SIZE; i++)
					{
						weightVectorW[i] = weightVectorW[i] - learningRateMu * delta * inputVectorX[i];

						if(i < IMAGE_SIZE * IMAGE_SIZE)
                        {
                            if(weightVectorW[i] > maxWeight)
                            {
                                maxWeight = weightVectorW[i];
                            }
                            if(weightVectorW[i] < minWeight)
                            {
                                minWeight = weightVectorW[i];
                            }
                        }
					}

					double offset = 0;

					if(minWeight < 0)
					{
						offset = fabs(minWeight);
					}

					double scale = 255 / fabs(maxWeight + offset);

					int arrayIndex = 0;

					for(int i = 0; i < IMAGE_SIZE; i++)
					{
						for(int j = 0; j < IMAGE_SIZE; j++)
						{
							((uchar *)(weightSpace->imageData + j*weightSpace->widthStep))[i*weightSpace->nChannels + 1] = (int)((weightVectorW[arrayIndex++] + offset) * scale);
						}
					}
				}


				int hitCount = 0;

				for(unsigned int i = 0; i < rate.size(); i++)
				{
					if(rate[i])
					{
						hitCount++;
					}
				}

				stringstream strs1, strs2;

				if(rate.size() == 100)
				{
					strs1 << "Correct hit rate: " << hitCount << "%";
					rate.erase(rate.begin());
				}
				else
				{
					strs1 << "Correct hit rate: " << (int)(((double)hitCount / (double)rate.size()) * 100) << "%";
				}

				strs2 << "Iteration: " << iteration;

				cvPutText(drawSpace, strs1.str().c_str(), cvPoint(32, 20), &font, CV_RGB(255, 255, 255));
				cvPutText(drawSpace, strs2.str().c_str(), cvPoint(55, drawSpace->height - 20), &font, CV_RGB(255, 255, 255));

				cvShowImage("Weights", weightSpace);
				cvShowImage("Plot", drawSpace);

				iteration++;
			}
		}
		else if(key == 'r')
		{
			memset(drawSpace->imageData, 0, drawSpace->height *  drawSpace->width * drawSpace->nChannels);
			cvShowImage("Plot", drawSpace);
		}
		else if(key == 'q')
		{
			break;
		}
	}

	cvDestroyAllWindows();
	cvReleaseImage(&drawSpace);
	cvReleaseImage(&weightSpace);

	return 0;
}

void init()
{
	drawSpace = cvCreateImage(cvSize(IMAGE_SIZE, IMAGE_SIZE), IPL_DEPTH_8U, 3);
	weightSpace = cvCreateImage(cvSize(IMAGE_SIZE, IMAGE_SIZE), IPL_DEPTH_8U, 3);

	memset(drawSpace->imageData, 0, drawSpace->width * drawSpace->height * drawSpace->nChannels);
	memset(weightSpace->imageData, 0, weightSpace->width * weightSpace->height * weightSpace->nChannels);

	cvStartWindowThread();
    cvNamedWindow("Plot", CV_WINDOW_AUTOSIZE);
    cvShowImage("Plot", drawSpace);
    cvNamedWindow("Weights", CV_WINDOW_AUTOSIZE);
    cvShowImage("Weights", weightSpace);

    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, 8);
}

void getInputX()
{
	int arrayIndex = 0;

	for(int i = 0; i < IMAGE_SIZE; i++)
	{
		for(int j = 0; j < IMAGE_SIZE; j++)
		{
			if(((uchar *)(drawSpace->imageData + j*drawSpace->widthStep))[i*drawSpace->nChannels + 0] == 255)
			{
				inputVectorX[arrayIndex] = 1;
			}
			else
			{
				inputVectorX[arrayIndex] = -1;
			}

			arrayIndex++;
		}
	}

	inputVectorX[IMAGE_SIZE * IMAGE_SIZE] = 1;
}

int calcF()
{
	double sum = 0.0;

	for(int i = 0; i < IMAGE_SIZE * IMAGE_SIZE + 1; i++)
	{
		sum += weightVectorW[i] * inputVectorX[i];
	}

	if(!initialised)
	{
		weightVectorW[IMAGE_SIZE * IMAGE_SIZE] = sum + 1;
	}

	return 2 * ((double)(sum > weightVectorW[IMAGE_SIZE * IMAGE_SIZE]) - 0.5);
}
