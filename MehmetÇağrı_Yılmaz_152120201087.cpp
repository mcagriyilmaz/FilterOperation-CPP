#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<cmath>
#include <iomanip>
#include <fstream>
#define MAX 21
#define FILTERSIZE 100

using namespace std;

void make_filter(double filter[][FILTERSIZE], int filter_size, ofstream& outFile);
void printFilter(double filter[][FILTERSIZE], int filter_size, ofstream& outFile);
void print_image(double image[][MAX], ofstream& outFile, int image_size);
void print_line(int size, ofstream& outFile);
void make_grid(double image[][MAX],int image_size,double grid_image[][MAX], int& loc);
void make_mirror(int image_size,double mirrored_image[][MAX], int& mirror_size, int filter_size, int loc);
void filtering(double mirrored_image[][MAX], int filter_size, double output_grid[][MAX], int loc, int image_size, double filter[][FILTERSIZE]);
void gridtoimage(double output_grid[][MAX], double output_image[][MAX],int image_size, int loc, int mirror_size);


void main() 
{
	int white, black;
	int i, j, mirror_size, loc;
	double image[MAX][MAX], filter[FILTERSIZE][FILTERSIZE] = {0}, output_grid[MAX][MAX] = { 0 };
	double grid_image[MAX][MAX] = { 0 }, output_image[MAX][MAX] = { 0 };
	double mirrored_image[MAX][MAX] = { 0 };
	int image_size, filter_size, grid_size=21;
	double value;

	ifstream inFile;
	ofstream outFile;

	inFile.open("inputs.txt");
	outFile.open("output.txt");

	inFile >> image_size;

	if (image_size%2==0||image_size>9)
	{
		outFile << "Image Size must be under 10 and odd!" << endl;
	}
	else
	{
		inFile >> filter_size;
		if (image_size % 2 == 0 || image_size > 9)
		{
			outFile << "Filter Size must be under 10 and odd!" << endl;
		}
		else
		{
			inFile >> white;
			inFile >> black;
			srand(time(NULL));
			for (i = 0; i < image_size; i++)
			{
				for (j = 0; j < image_size; j++)
				{
					image[i][j] = (rand() % white) + black;
				}
			}
			outFile << "Input Image (" << image_size << "x" << image_size << ")" << endl;
			print_line(image_size, outFile);
			print_image(image, outFile, image_size);
			print_line(image_size, outFile);
			outFile << endl << endl;

			outFile << "Gaussian Filter (" << filter_size << "x" << filter_size << ")" << endl;
			make_filter(filter, filter_size, outFile);
			print_line(filter_size, outFile);
			printFilter(filter, filter_size, outFile);
			print_line(filter_size, outFile);
			outFile << endl << endl << endl;

			outFile << "Input Grid: Edge Mirrored Image (21x21)" << endl;
			print_line(grid_size, outFile);
			make_grid(image, image_size, mirrored_image, loc);
			make_mirror(image_size, mirrored_image, mirror_size, filter_size, loc);
			print_image(mirrored_image, outFile, grid_size);
			print_line(grid_size, outFile);
			outFile << endl << endl;

			outFile << "Output Grid: Cutted-Edge Image (21x21)" << endl;
			print_line(grid_size, outFile);
			filtering(mirrored_image, filter_size, output_grid, loc, image_size, filter);
			print_image(output_grid, outFile, grid_size);
			print_line(grid_size, outFile);
			outFile << endl << endl;

			outFile << "Output(filtered) Image:(" << image_size << "x" << image_size << ")" << endl;
			print_line(image_size, outFile);
			gridtoimage(output_grid, output_image, image_size, loc, mirror_size);
			print_image(output_image, outFile, image_size);
			print_line(image_size, outFile);
			outFile << endl << endl;

		}
	}

	system("pause");
}
void make_filter(double filter[][FILTERSIZE], int filter_size, ofstream& outFile)
{
	outFile.setf(ios::fixed);
	outFile.precision(4);

	int f, d, i, j;
	double value;

	f = filter_size / 2;
	for (d = 0; d < f; d++)
	{
		value = 1.0 / pow((2 * (d + 1) + 1), 2);
		for (i = -(d + 1); i <= d + 1; i++)
		{
			for (j = -(d + 1); j <= d + 1; j++)
				filter[f - i][f - j] = filter[f - i][f - j] + value;
		}

	}


	for (i = 0; i < filter_size; i++)
	{
		for (j = 0; j < filter_size; j++)
		{
			filter[i][j] = filter[i][j] / static_cast<double>(filter_size);
		}
	}

}
void printFilter(double filter[][FILTERSIZE], int filter_size, ofstream& outFile)
{
	int i, j;

	for (i = 0; i < filter_size; i++)
	{
		for (j = 0; j < filter_size; j++)
		{
			outFile << right << setw(9) << filter[i][j];
		}
		outFile << endl;
	}
}
void print_image(double image[][MAX], ofstream& outFile, int image_size)
{
	int i, j;
	outFile.setf(ios::fixed);
	outFile.precision(4);

	for ( i = 0; i < image_size; i++)
	{
		for ( j = 0; j < image_size; j++)
		{
			outFile << right << setw(9) << image[i][j];
		}
		outFile << endl;
	}
}
void print_line(int size, ofstream& outFile)
{
	int i;
	for ( i = 0; i < size*9; i++)
	{
		outFile << "-";
	}
	outFile << endl;
}
void make_grid(double image[][MAX], int image_size, double grid_image[][MAX], int& loc)
{
	int i, j;
	loc = (21- image_size) / 2;

	for ( i = loc; i < image_size+loc; i++)
	{
		for ( j = loc; j < image_size+loc; j++)
		{
			grid_image[i][j] = image[i-loc][j -loc];
		}
	}
}
void make_mirror(int image_size, double mirrored_image[][MAX], int& mirror_size, int filter_size, int loc)
{
	int i, j;
	int new_size;
	mirror_size = filter_size / 2;
	new_size = image_size + (2 * mirror_size);

	for ( i = loc - mirror_size; i < loc-mirror_size+new_size; i++)
	{
		for ( j = loc - mirror_size; j < loc - mirror_size + new_size; j++)
		{
			if (i == loc)
				mirrored_image[i - 1][j] = mirrored_image[i][j];
			if (i == loc +image_size-1)
				mirrored_image[i + 1][j] = mirrored_image[i][j];
			if (j == loc)
				mirrored_image[i][j-1] = mirrored_image[i][j];
			if (j == loc + image_size - 1)
				mirrored_image[i][j+1] = mirrored_image[i][j];
		}
	}
	

	for (i = loc; i < loc+ image_size; i=i+ image_size-1)
	{
		for (j = loc; j <= loc + image_size; j = j + image_size-1)
		{
			if (i==loc)
			{
				if (j == loc)
					mirrored_image[i - 1][j - 1] = mirrored_image[i][j];
				else if (j == loc + image_size-1)
					mirrored_image[i - 1][j + 1] = mirrored_image[i][j];
			}
			if (i==loc+image_size-1)
			{
				if (j == loc)
					mirrored_image[i + 1][j - 1] = mirrored_image[i][j];
				else if (j == loc + image_size-1)
					mirrored_image[i + 1][j + 1] = mirrored_image[i][j];
			}

		}
	}

	if (mirror_size == 2 || mirror_size == 3 || mirror_size == 4)
	{
		for (i = loc - mirror_size; i < loc - mirror_size + new_size; i++)
		{
			for (j = loc - mirror_size; j < loc - mirror_size + new_size; j++)
			{
				if (i == loc - 1)
					mirrored_image[i - 1][j] = mirrored_image[i][j];
				if (i == loc + image_size)
					mirrored_image[i + 1][j] = mirrored_image[i][j];
				if (j == loc - 1)
					mirrored_image[i][j - 1] = mirrored_image[i][j];
				if (j == loc + image_size)
					mirrored_image[i][j + 1] = mirrored_image[i][j];
			
			}
		}
	}



	if (mirror_size == 3 || mirror_size == 4)
	{
		for (i = loc - mirror_size; i < loc - mirror_size + new_size; i++)
		{
			for (j = loc - mirror_size; j < loc - mirror_size + new_size; j++)
			{
				if (i == loc - 2)
					mirrored_image[i - 1][j] = mirrored_image[i][j];
				if (i == loc + image_size + 1)
					mirrored_image[i + 1][j] = mirrored_image[i][j];
				if (j == loc - 2)
					mirrored_image[i][j - 1] = mirrored_image[i][j];
				if (j == loc + image_size + 1)
					mirrored_image[i][j + 1] = mirrored_image[i][j];
			}
		}
	}
	if (mirror_size == 4)
	{
		for (i = loc - mirror_size; i < loc - mirror_size + new_size; i++)
		{
			for (j = loc - mirror_size; j < loc - mirror_size + new_size; j++)
			{
				if (i == loc - 3)
					mirrored_image[i - 1][j] = mirrored_image[i][j];
				if (i == loc + image_size + 2)
					mirrored_image[i + 1][j] = mirrored_image[i][j];
				if (j == loc - 3)
					mirrored_image[i][j - 1] = mirrored_image[i][j];
				if (j == loc + image_size + 2)
					mirrored_image[i][j + 1] = mirrored_image[i][j];
			}
		}
	}
}
void filtering(double mirrored_image[][MAX], int filter_size, double output_grid[][MAX], int loc,int image_size, double filter[][FILTERSIZE])
{
	int i = 0, j = 0;
	int k = 0, l = 0;
	int a, b;
	double value;
	int filter_capasity, mirror_size;
	double sum = 0;
	filter_capasity = filter_size * filter_size;
	mirror_size = filter_size / 2;
	
	for (int i = loc-mirror_size; i < loc - mirror_size+image_size; i++) {
		for (int j = loc - mirror_size; j < loc - mirror_size + image_size; j++) {
			int m = i;
			for (int k = 0; k < filter_size; k++) {
				int n = j;
				for (int l = 0; l < filter_size; l++) {
					sum = sum + (mirrored_image[m][n] * filter[k][l]);
					n++;
				}
				m++;

			}
			output_grid[i+mirror_size][j+mirror_size] = sum / filter_capasity;
			sum = 0;
		}
	}



}
void gridtoimage(double output_grid[][MAX], double output_image[][MAX],int image_size, int loc,int mirror_size)
{
	int i, j;

	for ( i = 0; i < image_size; i++)
	{
		for (j = 0; j < image_size; j++)
		{
			output_image[i][j] = output_grid[i + loc][j + loc];
		}
	}
}


