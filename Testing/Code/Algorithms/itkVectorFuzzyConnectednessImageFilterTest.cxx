/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorFuzzyConnectednessImageFilterTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#include "itkVectorFuzzyConnectednessImageFilter.h"


const int LENGTH = 1;
const int HEIGHT = 20;
const int WIDTH = 20;

const int objects_num = 2;
const int selected_object = 1;

int mean1[3] = {213,194,140};
int mean2[3] = {83,50, 27};


double object_cov1[3][3] = 
	{
		{255.27,  340.36,  268.36},
		{340.36,  514.63,  422.18},
		{268.36,  422.18,  358.63},
	};
double object_cov2[3][3] = 
	{
		{213.54,  -28.63, -21.27},
		{-28.63,  193.5,  132.14},
		{-21.27,  132.14, 100.63},
	};

unsigned long Seed1[1][3] = 
	{
		{5, 15,0},
	};
unsigned long Seed2[1][3] = 
	{
		{15,5, 0},
	};

// this data array is from part of visible human data
const int data[400][3] = 
{ 
	{201, 178, 127}, {201, 177, 131}, {198, 178, 131}, {196, 172, 123}, {190, 148, 93}, {175, 117, 67}, {153, 96, 47}, {133, 65, 33}, {106, 56, 32}, {91, 47, 26}, 
	{81, 45, 25}, {79, 42, 23}, {74, 38, 21}, {69, 35, 19}, {64, 39, 19}, {68, 51, 26}, {80, 62, 38}, {89, 66, 40}, {94, 62, 33}, {88, 52, 29}, 
	{206, 184, 133}, {205, 183, 134}, {201, 184, 135}, {199, 177, 129}, {193, 162, 108}, {184, 129, 75}, {163, 105, 55}, {141, 72, 33}, {112, 58, 28}, {95, 58, 32}, 
	{92, 50, 28}, {84, 45, 27}, {77, 40, 24}, {69, 36, 19}, {64, 47, 23}, {74, 50, 26}, {78, 66, 50}, {88, 69, 41}, {94, 57, 30}, {84, 57, 34}, 
	{207, 185, 134}, {207, 188, 138}, {205, 188, 139}, {202, 183, 134}, {197, 169, 119}, {187, 138, 80}, {166, 106, 55}, {143, 79, 35}, {118, 62, 32}, {101, 62, 36}, 
	{97, 57, 32}, {87, 46, 25}, {77, 39, 21}, {69, 40, 22}, {69, 52, 28}, {79, 50, 26}, {77, 46, 25}, {74, 60, 35}, {88, 60, 34}, {88, 59, 32}, 
	{211, 188, 133}, {209, 191, 140}, {207, 192, 142}, {205, 187, 138}, {199, 177, 126}, {192, 150, 90}, {174, 114, 61}, {148, 88, 43}, {125, 60, 28}, {103, 63, 35}, 
	{102, 60, 32}, {91, 50, 27}, {79, 41, 24}, {73, 58, 31}, {85, 56, 29}, {81, 52, 25}, {81, 49, 24}, {77, 54, 30}, {82, 60, 35}, {87, 55, 30}, 
	{215, 193, 136}, {212, 195, 141}, {209, 194, 142}, {208, 191, 140}, {202, 181, 132}, {196, 161, 103}, {181, 129, 74}, {160, 102, 50}, {135, 68, 32}, {108, 61, 30}, 
	{101, 57, 33}, {93, 53, 29}, {83, 45, 25}, {77, 59, 32}, {86, 59, 33}, {86, 55, 27}, {85, 52, 28}, {79, 52, 29}, {80, 59, 35}, {86, 48, 27}, 
	{216, 197, 141}, {214, 197, 140}, {211, 193, 139}, {208, 191, 139}, {203, 184, 133}, {197, 171, 114}, {187, 139, 82}, {168, 116, 65}, {146, 80, 38}, {116, 60, 29}, 
	{98, 60, 35}, {92, 54, 29}, {84, 47, 25}, {77, 58, 34}, {84, 59, 32}, {85, 52, 26}, {81, 47, 25}, {77, 54, 31}, {82, 55, 30}, {82, 43, 23}, 
	{218, 200, 143}, {215, 199, 143}, {214, 198, 139}, {210, 193, 138}, {206, 187, 136}, {200, 177, 124}, {192, 150, 89}, {174, 120, 70}, {152, 92, 43}, {127, 68, 36}, 
	{105, 64, 38}, {96, 46, 25}, {80, 50, 28}, {81, 56, 33}, {84, 50, 26}, {80, 48, 24}, {78, 49, 27}, {78, 59, 36}, {85, 47, 25}, {74, 43, 24}, 
	{219, 205, 146}, {218, 200, 145}, {215, 200, 143}, {213, 197, 142}, {208, 192, 137}, {203, 183, 129}, {197, 165, 107}, {184, 129, 73}, {160, 109, 59}, {140, 75, 37}, 
	{112, 59, 30}, {93, 47, 25}, {83, 51, 27}, {84, 50, 27}, {82, 46, 24}, {78, 46, 23}, {78, 46, 28}, {76, 52, 30}, {78, 43, 24}, {70, 44, 24}, 
	{220, 205, 147}, {219, 202, 146}, {217, 202, 146}, {214, 198, 145}, {209, 195, 142}, {204, 189, 136}, {200, 174, 118}, {189, 142, 82}, {167, 115, 66}, {146, 83, 41}, 
	{120, 56, 26}, {93, 47, 25}, {82, 50, 27}, {82, 48, 24}, {81, 45, 24}, {78, 43, 23}, {74, 45, 27}, {73, 46, 26}, {73, 48, 29}, {74, 50, 27}, 
	{220, 205, 147}, {219, 204, 146}, {218, 204, 148}, {215, 200, 145}, {211, 198, 144}, {207, 192, 141}, {204, 182, 128}, {195, 158, 98}, {178, 123, 72}, {153, 98, 49}, 
	{131, 67, 35}, {105, 55, 28}, {88, 45, 24}, {79, 44, 23}, {79, 48, 26}, {80, 46, 25}, {77, 52, 32}, {80, 46, 27}, {75, 49, 28}, {77, 51, 27}, 
	{221, 205, 146}, {220, 203, 145}, {217, 205, 148}, {216, 201, 146}, {214, 199, 146}, {210, 195, 144}, {206, 188, 135}, {199, 170, 117}, {187, 138, 79}, {165, 107, 60}, 
	{145, 81, 42}, {118, 61, 29}, {99, 48, 23}, {83, 49, 24}, {83, 49, 26}, {80, 52, 31}, {83, 53, 31}, {82, 57, 32}, {83, 54, 30}, {82, 39, 21}, 
	{223, 206, 146}, {221, 204, 146}, {220, 204, 148}, {217, 203, 146}, {216, 200, 146}, {213, 199, 146}, {208, 191, 139}, {202, 179, 127}, {192, 158, 102}, {178, 121, 68}, 
	{154, 98, 52}, {136, 76, 34}, {115, 63, 33}, {99, 63, 34}, {96, 58, 29}, {91, 52, 30}, {84, 52, 30}, {82, 49, 28}, {78, 43, 22}, {73, 37, 19}, 
	{222, 207, 148}, {222, 204, 147}, {222, 203, 147}, {219, 203, 147}, {217, 200, 146}, {213, 199, 145}, {210, 192, 141}, {205, 185, 134}, {199, 171, 119}, {189, 139, 83}, 
	{168, 112, 63}, {148, 97, 46}, {132, 72, 34}, {111, 61, 35}, {99, 60, 31}, {94, 54, 30}, {85, 50, 27}, {81, 41, 21}, {73, 38, 19}, {68, 42, 23}, 
	{224, 207, 150}, {222, 206, 148}, {222, 204, 147}, {220, 202, 146}, {217, 200, 144}, {215, 197, 143}, {212, 194, 141}, {209, 188, 136}, {203, 180, 129}, {196, 157, 101}, 
	{180, 117, 63}, {155, 103, 56}, {142, 83, 39}, {123, 64, 30}, {103, 52, 25}, {89, 51, 29}, {85, 48, 25}, {82, 51, 27}, {82, 44, 23}, {76, 42, 22}, 
	{224, 209, 153}, {223, 207, 151}, {222, 205, 148}, {220, 201, 145}, {219, 200, 145}, {216, 198, 144}, {214, 195, 142}, {210, 190, 137}, {207, 183, 131}, {201, 173, 120}, 
	{192, 146, 87}, {173, 113, 62}, {152, 101, 54}, {139, 83, 40}, {120, 61, 28}, {101, 50, 25}, {86, 49, 26}, {83, 54, 31}, {87, 51, 26}, {83, 43, 21}, 
	{223, 208, 153}, {224, 206, 152}, {222, 204, 149}, {220, 202, 145}, {219, 200, 143}, {217, 199, 144}, {215, 197, 143}, {212, 193, 139}, {210, 186, 133}, {205, 179, 128}, 
	{197, 166, 114}, {187, 140, 84}, {169, 109, 61}, {148, 99, 52}, {136, 83, 38}, {120, 70, 34}, {107, 68, 34}, {101, 62, 33}, {95, 53, 26}, {87, 49, 25}, 
	{223, 207, 152}, {223, 205, 150}, {222, 203, 148}, {220, 200, 145}, {219, 201, 143}, {220, 198, 143}, {216, 198, 144}, {216, 195, 140}, {212, 188, 135}, {207, 183, 134}, 
	{203, 178, 128}, {195, 161, 108}, {183, 129, 75}, {162, 104, 60}, {145, 93, 50}, {132, 86, 46}, {123, 85, 47}, {117, 70, 35}, {105, 60, 27}, {96, 49, 28}, 
	{223, 204, 147}, {222, 203, 147}, {222, 202, 146}, {221, 203, 146}, {220, 200, 146}, {220, 200, 145}, {218, 196, 142}, {215, 194, 140}, {213, 190, 138}, {209, 187, 137}, 
	{206, 182, 133}, {200, 173, 123}, {193, 157, 103}, {180, 127, 74}, {160, 98, 52}, {141, 93, 53}, {131, 86, 49}, {124, 82, 49}, {117, 70, 34}, {105, 55, 29}, 
	{223, 200, 146}, {222, 200, 146}, {222, 202, 148}, {221, 202, 149}, {221, 201, 147}, {220, 201, 146}, {219, 197, 144}, {218, 194, 141}, {214, 191, 138}, {210, 187, 134}, 
	{210, 185, 136}, {206, 182, 134}, {200, 172, 126}, {193, 156, 105}, {181, 128, 74}, {161, 103, 57}, {144, 94, 52}, {133, 85, 42}, {124, 79, 42}, {116, 69, 34}, 
	{222, 198, 146}, {222, 199, 147}, {221, 198, 148}, {220, 200, 149}, {221, 200, 148}, {218, 199, 145}, {219, 197, 143}, {219, 195, 141}, {217, 193, 140}, {215, 189, 137}, 
	{212, 189, 138}, {210, 186, 140}, {206, 181, 134}, {201, 169, 123}, {191, 155, 105}, {182, 134, 81}, {166, 110, 64}, {149, 98, 55}, {137, 93, 48}, {130, 79, 36}, 
};

int main()
{
	
	typedef itk::Vector<int,3> IntVector;
	typedef itk::Matrix<double,3>  MatrixType;
	typedef itk::Image<bool,3> BinaryImage3D;
	typedef itk::Image<IntVector,3> VectorImage3D;
	typedef itk::VectorFuzzyConnectednessImageFilter<VectorImage3D,BinaryImage3D> FuzzyImage;

	itk::Size<3> size = {WIDTH, HEIGHT, LENGTH};
	double spacing[3] = { 0.33,  0.33, 1.0};
	
	FuzzyImage::Pointer testFuzzy=FuzzyImage::New();
	VectorImage3D::Pointer inputimg=VectorImage3D::New();
	VectorImage3D::IndexType index=VectorImage3D::IndexType::ZeroIndex;
	VectorImage3D::RegionType region;

	region.SetSize(size);
	region.SetIndex(index);

	inputimg->SetLargestPossibleRegion( region );
	inputimg->SetBufferedRegion( region );
	inputimg->SetRequestedRegion( region );
	inputimg->SetSpacing(spacing);
	inputimg->Allocate();

/*  local test passed

	FILE *fin, *fout;
	char Input[80],output[80];
	strcpy(output,"bin.raw");
	strcpy(Input, "fat.raw");
	unsigned char *data;

	fin = fopen(Input,"rb");
	data = new unsigned char[DEEP*HEIGHT*WIDTH*3];
	fread(data,1,DEEP*HEIGHT*WIDTH*3,fin);
*/
	itk::ImageRegionIteratorWithIndex <VectorImage3D> it(inputimg, region);

	int k=0;
	IntVector value;

	while( !it.IsAtEnd()) 
		{
			value[0] = data[k][0];
			value[1] = data[k][1];
			value[2] = data[k][2];
			k = k+1;
			it.Set(value);
			++it;
		}

//	fclose(fin);
//	delete data;
    	
	testFuzzy->SetInput(inputimg);
	testFuzzy->SetObjects(objects_num);
	testFuzzy->SetSelectedObject(selected_object);
	testFuzzy->Initialization();

	testFuzzy->SetObjectsMean(mean1,0);
	testFuzzy->SetObjectsMean(mean2,1);

	MatrixType matrix1;
	matrix1.GetVnlMatrix().set((double *)object_cov1);
	testFuzzy->SetObjectsMatrix(matrix1,0);
	matrix1.GetVnlMatrix().set((double *)object_cov2);
	testFuzzy->SetObjectsMatrix(matrix1,1);


	for ( int i = 0; i < 1; i++) 
		{
			index.SetIndex(Seed1[i]);
			testFuzzy->SetObjectsSeed(index,0);
    }

	for ( int i = 0; i < 1; i++) 
		{
			index.SetIndex(Seed2[i]);
			testFuzzy->SetObjectsSeed(index,1);
    }
	
	testFuzzy->DoFuzzySegmentation();

	itk::ImageRegionIteratorWithIndex <BinaryImage3D> ot(testFuzzy->GetOutput(), region);

	ot.GoToBegin();

	for(int i = 0;i < LENGTH*HEIGHT*WIDTH; i++)
		{
			if((i%WIDTH) == 0)
				std::cout<<std::endl;
			std::cout<<ot.Get();
			++ot;
		}

	return 0;
}
