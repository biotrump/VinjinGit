/***************************************************************************
*   Copyright (C) 2006 by Mian Zhou   *
*   M.Zhou@reading.ac.uk   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#include "cvgabor.h"

CvGabor::CvGabor()
{
}


CvGabor::~CvGabor()
{
	cvReleaseMat( &Real );
	cvReleaseMat( &Imag );
}




/*!
\fn CvGabor::CvGabor(int iMu, int iNu, double dSigma)
Construct a gabor

Parameters:
iMu      The orientation iMu*PI/8,
iNu       The scale,
dSigma       The sigma value of Gabor,

Returns:
None

Create a gabor with a orientation iMu*PI/8, a scale iNu, and a sigma value dSigma. The spatial frequence (F) is set to sqrt(2) defaultly. It calls Init() to generate parameters and kernels.
*/
CvGabor::CvGabor(int iMu, int iNu, double dSigma)
{ 
	F = sqrt(2.0);
	Init(iMu, iNu, dSigma, F);
}


/*!
\fn CvGabor::CvGabor(int iMu, int iNu, double dSigma, double dF)
Construct a gabor

Parameters:
iMu      The orientation iMu*PI/8
iNu       The scale
dSigma       The sigma value of Gabor
dF      The spatial frequency 

Returns:
None

Create a gabor with a orientation iMu*PI/8, a scale iNu, a sigma value dSigma, and a spatial frequence dF. It calls Init() to generate parameters and kernels.
*/
CvGabor::CvGabor(int iMu, int iNu, double dSigma, double dF)
{

	Init(iMu, iNu, dSigma, dF);

}


/*!
\fn CvGabor::CvGabor(double dPhi, int iNu)
Construct a gabor

Parameters:
dPhi      The orientation in arc
iNu       The scale

Returns:
None

Create a gabor with a orientation dPhi, and with a scale iNu. The sigma (Sigma) and the spatial frequence (F) are set to 2*PI and sqrt(2) defaultly. It calls Init() to generate parameters and kernels.
*/
CvGabor::CvGabor(double dPhi, int iNu)
{
	Sigma = 2*PI;
	F = sqrt(2.0);
	Init(dPhi, iNu, Sigma, F);
}


/*!
\fn CvGabor::CvGabor(double dPhi, int iNu, double dSigma)
Construct a gabor

Parameters:
dPhi      The orientation in arc
iNu       The scale
dSigma      The sigma value of Gabor

Returns:
None

Create a gabor with a orientation dPhi, a scale iNu, and a sigma value dSigma. The spatial frequence (F) is set to sqrt(2) defaultly. It calls Init() to generate parameters and kernels.
*/
CvGabor::CvGabor(double dPhi, int iNu, double dSigma)
{

	F = sqrt(2.0);
	Init(dPhi, iNu, dSigma, F);
}


/*!
\fn CvGabor::CvGabor(double dPhi, int iNu, double dSigma, double dF)
Construct a gabor

Parameters:
dPhi      The orientation in arc
iNu       The scale
dSigma       The sigma value of Gabor
dF      The spatial frequency 

Returns:
None

Create a gabor with a orientation dPhi, a scale iNu, a sigma value dSigma, and a spatial frequence dF. It calls Init() to generate parameters and kernels.
*/
CvGabor::CvGabor(double dPhi, int iNu, double dSigma, double dF)
{

	Init(dPhi, iNu, dSigma,dF);
}

/*!
\fn CvGabor::IsInit()
Determine the gabor is initilised or not

Parameters:
None

Returns:
a boolean value, TRUE is initilised or FALSE is non-initilised.

Determine whether the gabor has been initlized - variables F, K, Kmax, Phi, Sigma are filled.
*/
bool CvGabor::IsInit()
{

	return bInitialised;
}

/*!
\fn CvGabor::mask_width()
Give out the width of the mask

Parameters:
None

Returns:
The long type show the width.

Return the width of mask (should be NxN) by the value of Sigma and iNu.
*/
long CvGabor::mask_width()
{

	long lWidth;
	if (IsInit() == false)  {
		perror ("Error: The Object has not been initilised in mask_width()!\n");
		return 0;
	}
	else {
		//determine the width of Mask
		double dModSigma = Sigma/K;
		double dWidth = cvRound(dModSigma*6 + 1);

		//test whether dWidth is an odd.
		if (fmod(dWidth, 2.0)==0.0) dWidth++;
		lWidth = (long)dWidth;

		return lWidth;
	}
}


/*!
\fn CvGabor::creat_kernel()
Create gabor kernel

Parameters:
None

Returns:
None

Create 2 gabor kernels - REAL and IMAG, with an orientation and a scale 
*/
void CvGabor::creat_kernel()
{

	if (IsInit() == false) {perror("Error: The Object has not been initilised in creat_kernel()!\n");}
	else {
		CvMat *mReal, *mImag;
		mReal = cvCreateMat( Width, Width, CV_32FC1);
		mImag = cvCreateMat( Width, Width, CV_32FC1);

		/**************************** Gabor Function ****************************/ 
		int x, y;
		double dReal;
		double dImag;
		double dTemp1, dTemp2, dTemp3;

		for (int i = 0; i < Width; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				x = i-(Width-1)/2;
				y = j-(Width-1)/2;
				dTemp1 = (pow(K,2)/pow(Sigma,2))*exp(-(pow((double)x,2)+pow((double)y,2))*pow(K,2)/(2*pow(Sigma,2)));
				dTemp2 = cos(K*cos(Phi)*x + K*sin(Phi)*y) - exp(-(pow(Sigma,2)/2));
				dTemp3 = sin(K*cos(Phi)*x + K*sin(Phi)*y);
				dReal = dTemp1*dTemp2;
				dImag = dTemp1*dTemp3; 
				//gan_mat_set_el(pmReal, i, j, dReal);
				//cvmSet( (CvMat*)mReal, i, j, dReal );
				cvSetReal2D((CvMat*)mReal, i, j, dReal );
				//gan_mat_set_el(pmImag, i, j, dImag);
				//cvmSet( (CvMat*)mImag, i, j, dImag );
				cvSetReal2D((CvMat*)mImag, i, j, dImag );

			} 
		}
		/**************************** Gabor Function ****************************/
		bKernel = true;
		cvCopy(mReal, Real, NULL);
		cvCopy(mImag, Imag, NULL);
		//printf("A %d x %d Gabor kernel with %f PI in arc is created.\n", Width, Width, Phi/PI);
		cvReleaseMat( &mReal );
		cvReleaseMat( &mImag );
	}
}


/*!
\fn CvGabor::get_image(int Type)
Get the speific type of image of Gabor

Parameters:
Type      The Type of gabor kernel, e.g. REAL, IMAG, MAG, PHASE   

Returns:
Pointer to image structure, or NULL on failure   

Return an Image (gandalf image class) with a specific Type   "REAL"   "IMAG" "MAG" "PHASE"  
*/
IplImage* CvGabor::get_image(int Type)
{

	if(IsKernelCreate() == false)
	{ 
		perror("Error: the Gabor kernel has not been created in get_image()!\n");
		return NULL;
	}
	else
	{  
		IplImage* pImage;
		IplImage *newimage;
		newimage = cvCreateImage(cvSize(Width,Width), IPL_DEPTH_8U, 1 );
		//printf("Width is %d.\n",(int)Width);
		//printf("Sigma is %f.\n", Sigma);
		//printf("F is %f.\n", F);
		//printf("Phi is %f.\n", Phi);

		//pImage = gan_image_alloc_gl_d(Width, Width);
		pImage = cvCreateImage( cvSize(Width,Width), IPL_DEPTH_32F, 1 );


		CvMat* kernel = cvCreateMat(Width, Width, CV_32FC1);
		CvMat* re = cvCreateMat(Width, Width, CV_32FC1);
		CvMat* im = cvCreateMat(Width, Width, CV_32FC1);
		double ve, ve1,ve2;
		CvSize size = cvGetSize( kernel );
		int rows = size.height;
		int cols = size.width;
		switch(Type)
		{
		case 1:  //Real

			cvCopy( (CvMat*)Real, (CvMat*)kernel, NULL );
			//pImage = cvGetImage( (CvMat*)kernel, pImageGL );
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					ve = cvGetReal2D((CvMat*)kernel, i, j);
					cvSetReal2D( (IplImage*)pImage, j, i, ve );
				}
			}
			break;
		case 2:  //Imag
			cvCopy( (CvMat*)Imag, (CvMat*)kernel, NULL );
			//pImage = cvGetImage( (CvMat*)kernel, pImageGL );
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					ve = cvGetReal2D((CvMat*)kernel, i, j);
					cvSetReal2D( (IplImage*)pImage, j, i, ve );
				}
			}
			break; 
		case 3:  //Magnitude //add by yao

			cvCopy( (CvMat*)Real, (CvMat*)re, NULL );
			cvCopy( (CvMat*)Imag, (CvMat*)im, NULL );
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					ve1 = cvGetReal2D((CvMat*)re, i, j);
					ve2 = cvGetReal2D((CvMat*)im, i, j);
					ve = cvSqrt(ve1*ve1+ve2*ve2);
					cvSetReal2D( (IplImage*)pImage, j, i, ve );
				}
			}
			break;
		case 4:  //Phase
			///@todo
			break;
		}

		cvNormalize((IplImage*)pImage, (IplImage*)pImage, 0, 255, CV_MINMAX, NULL );


		cvConvertScaleAbs( (IplImage*)pImage, (IplImage*)newimage, 1, 0 );

		cvReleaseMat(&kernel);

		cvReleaseImage(&pImage);

		return newimage;
	}
}


/*!
\fn CvGabor::IsKernelCreate()
Determine the gabor kernel is created or not

Parameters:
None

Returns:
a boolean value, TRUE is created or FALSE is non-created.

Determine whether a gabor kernel is created.
*/
bool CvGabor::IsKernelCreate()
{

	return bKernel;
}


/*!
\fn CvGabor::get_mask_width()
Reads the width of Mask

Parameters:
None

Returns:
Pointer to long type width of mask.
*/
long CvGabor::get_mask_width()
{
	return Width;
}


/*!
\fn CvGabor::Init(int iMu, int iNu, double dSigma, double dF)
Initilize the.gabor

Parameters:
iMu    The orientations is iMu*PI/8
iNu    The scale can be from -5 to infinit
dSigma    The Sigma value of gabor, Normally set to 2*PI
dF    The spatial frequence , normally is sqrt(2)

Returns:

Initilize the.gabor with the orientation iMu, the scale iNu, the sigma dSigma, the frequency dF, it will call the function creat_kernel(); So a gabor is created.
*/
void CvGabor::Init(int iMu, int iNu, double dSigma, double dF)
{
	//Initilise the parameters 
	bInitialised = false;
	bKernel = false;

	Sigma = dSigma;
	F = dF;

	Kmax = PI/2;

	// Absolute value of K
	K = Kmax / pow(F, (double)iNu);
	Phi = PI*iMu/8;
	bInitialised = true;
	Width = mask_width();
	Real = cvCreateMat( Width, Width, CV_32FC1);
	Imag = cvCreateMat( Width, Width, CV_32FC1);
	creat_kernel();  
}

/*!
\fn CvGabor::Init(double dPhi, int iNu, double dSigma, double dF)
Initilize the.gabor

Parameters:
dPhi    The orientations 
iNu    The scale can be from -5 to infinit
dSigma    The Sigma value of gabor, Normally set to 2*PI
dF    The spatial frequence , normally is sqrt(2)

Returns:
None

Initilize the.gabor with the orientation dPhi, the scale iNu, the sigma dSigma, the frequency dF, it will call the function creat_kernel(); So a gabor is created.filename    The name of the image file
file_format    The format of the file, e.g. GAN_PNG_FORMAT
image    The image structure to be written to the file
octrlstr    Format-dependent control structure

*/
void CvGabor::Init(double dPhi, int iNu, double dSigma, double dF)
{

	bInitialised = false;
	bKernel = false;
	Sigma = dSigma;
	F = dF;

	Kmax = PI/2;

	// Absolute value of K
	K = Kmax / pow(F, (double)iNu);
	Phi = dPhi;
	bInitialised = true;
	Width = mask_width();
	Real = cvCreateMat( Width, Width, CV_32FC1);
	Imag = cvCreateMat( Width, Width, CV_32FC1);
	creat_kernel();  
}



/*!
\fn CvGabor::get_matrix(int Type)
Get a matrix by the type of kernel

Parameters:
Type      The type of kernel, e.g. REAL, IMAG, MAG, PHASE

Returns:
Pointer to matrix structure, or NULL on failure.

Return the gabor kernel.
*/
CvMat* CvGabor::get_matrix(int Type)
{
	if (!IsKernelCreate()) {perror("Error: the gabor kernel has not been created!\n"); return NULL;}
	switch (Type)
	{
	case CV_GABOR_REAL:
		return Real;
	case CV_GABOR_IMAG:
		return Imag;
	case CV_GABOR_MAG:
		return NULL;
	case CV_GABOR_PHASE:
		return NULL;
	default:
		return NULL;
	}   
}




/*!
\fn CvGabor::output_file(const char *filename, Gan_ImageFileFormat file_format, int Type)
Writes a gabor kernel as an image file.

Parameters:
filename    The name of the image file
file_format    The format of the file, e.g. GAN_PNG_FORMAT
Type      The Type of gabor kernel, e.g. REAL, IMAG, MAG, PHASE   
Returns:
None

Writes an image from the provided image structure into the given file and the type of gabor kernel.
*/
void CvGabor::output_file(const char *filename, int Type)
{
	IplImage *pImage;
	pImage = get_image(Type);
	if(pImage != NULL)
	{
		if( cvSaveImage(filename, pImage )) printf("%s has been written successfully!\n", filename);
		else printf("Error: writting %s has failed!\n", filename);
	}
	else 
		perror("Error: the image is empty in output_file()!\n"); 

	cvReleaseImage(&pImage);
}






/*!
\fn CvGabor::show(int Type)
*/
void CvGabor::show(int Type)
{
	if(!IsInit()) {
		perror("Error: the gabor kernel has not been created!\n");
	}
	else {
		IplImage *pImage;
		pImage = get_image(Type);
		cvNamedWindow("Testing",1);
		cvShowImage("Testing",pImage);
		cvWaitKey(0);
		cvDestroyWindow("Testing");
		cvReleaseImage(&pImage);
	}

}




/*!
\fn CvGabor::conv_img_a(IplImage *src, IplImage *dst, int Type)
*/
void CvGabor::conv_img_a(IplImage *src, IplImage *dst, int Type)
{
	double ve, re,im;

	int width = src->width;
	int height = src->height;
	CvMat *mat = cvCreateMat(src->width, src->height, CV_32FC1);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			ve = cvGetReal2D((IplImage*)src, j, i);
			cvSetReal2D( (CvMat*)mat, i, j, ve );
		}
	}

	CvMat *rmat = cvCreateMat(width, height, CV_32FC1);
	CvMat *imat = cvCreateMat(width, height, CV_32FC1);

	CvMat *kernel = cvCreateMat( Width, Width, CV_32FC1 );

	switch (Type)
	{
	case CV_GABOR_REAL:
		cvCopy( (CvMat*)Real, (CvMat*)kernel, NULL );
		cvFilter2D( (CvMat*)mat, (CvMat*)mat, (CvMat*)kernel, cvPoint( (Width-1)/2, (Width-1)/2));
		break;
	case CV_GABOR_IMAG:
		cvCopy( (CvMat*)Imag, (CvMat*)kernel, NULL );
		cvFilter2D( (CvMat*)mat, (CvMat*)mat, (CvMat*)kernel, cvPoint( (Width-1)/2, (Width-1)/2));
		break;
	case CV_GABOR_MAG:
		/* Real Response */
		cvCopy( (CvMat*)Real, (CvMat*)kernel, NULL );
		cvFilter2D( (CvMat*)mat, (CvMat*)rmat, (CvMat*)kernel, cvPoint( (Width-1)/2, (Width-1)/2));
		/* Imag Response */
		cvCopy( (CvMat*)Imag, (CvMat*)kernel, NULL );
		cvFilter2D( (CvMat*)mat, (CvMat*)imat, (CvMat*)kernel, cvPoint( (Width-1)/2, (Width-1)/2));
		/* Magnitude response is the square root of the sum of the square of real response and imaginary response */
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				re = cvGetReal2D((CvMat*)rmat, i, j);
				im = cvGetReal2D((CvMat*)imat, i, j);
				ve = sqrt(re*re + im*im);
				cvSetReal2D( (CvMat*)mat, i, j, ve );
			}
		}       
		break;
	case CV_GABOR_PHASE:
		break;
	}

	if (dst->depth == IPL_DEPTH_8U)
	{
		cvNormalize((CvMat*)mat, (CvMat*)mat, 0, 255, CV_MINMAX, NULL);
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				ve = cvGetReal2D((CvMat*)mat, i, j);
				ve = cvRound(ve);
				cvSetReal2D( (IplImage*)dst, j, i, ve );
			}
		}
	}

	if (dst->depth == IPL_DEPTH_32F)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				ve = cvGetReal2D((CvMat*)mat, i, j);
				cvSetReal2D( (IplImage*)dst, j, i, ve );
			}
		}
	} 

	cvReleaseMat(&kernel);
	cvReleaseMat(&imat);
	cvReleaseMat(&rmat);
	cvReleaseMat(&mat);
}


/*!
\fn CvGabor::CvGabor(int iMu, int iNu)
*/
CvGabor::CvGabor(int iMu, int iNu)
{
	double dSigma = 2*PI; 
	F = sqrt(2.0);
	Init(iMu, iNu, dSigma, F);
}


/*!
\fn CvGabor::normalize( const CvArr* src, CvArr* dst, double a, double b, int norm_type, const CvArr* mask )
*/
void CvGabor::normalize( const CvArr* src, CvArr* dst, double a, double b, int norm_type, const CvArr* mask )
{
	CvMat* tmp = 0;
	__CV_BEGIN__;

	double scale, shift;

	if( norm_type == CV_MINMAX )
	{
		double smin = 0, smax = 0;
		double dmin = MIN( a, b ), dmax = MAX( a, b );
		cvMinMaxLoc( src, &smin, &smax, 0, 0, mask );
		scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
		shift = dmin - smin*scale;
	}
	else if( norm_type == CV_L2 || norm_type == CV_L1 || norm_type == CV_C )
	{
		CvMat *s = (CvMat*)src, *d = (CvMat*)dst;

		scale = cvNorm( src, 0, norm_type, mask );
		scale = scale > DBL_EPSILON ? 1./scale : 0.;
		shift = 0;
	}
	else {}



	if( !mask )
		cvConvertScale( src, dst, scale, shift );
	else
	{
		cvConvertScale( src, tmp, scale, shift );
		cvCopy( tmp, dst, mask );
	}

	__CV_END__;

	if( tmp )
		cvReleaseMat( &tmp );
}


/*!
\fn CvGabor::conv_img(IplImage *src, IplImage *dst, int Type)
*/
void CvGabor::conv_img(IplImage *src, IplImage *dst, int Type)
{
	double ve;

	CvMat *mat = cvCreateMat(src->width, src->height, CV_32FC1);
	for (int i = 0; i < src->width; i++)
	{
		for (int j = 0; j < src->height; j++)
		{
			ve = CV_IMAGE_ELEM(src, uchar, j, i);
			CV_MAT_ELEM(*mat, float, i, j) = (float)ve;
		}
	}

	CvMat *rmat = cvCreateMat(src->width, src->height, CV_32FC1);
	CvMat *imat = cvCreateMat(src->width, src->height, CV_32FC1);

	switch (Type)
	{
	case CV_GABOR_REAL:
		cvFilter2D( (CvMat*)mat, (CvMat*)mat, (CvMat*)Real, cvPoint( (Width-1)/2, (Width-1)/2));
		break;
	case CV_GABOR_IMAG:
		cvFilter2D( (CvMat*)mat, (CvMat*)mat, (CvMat*)Imag, cvPoint( (Width-1)/2, (Width-1)/2));
		break;
	case CV_GABOR_MAG:
		cvFilter2D( (CvMat*)mat, (CvMat*)rmat, (CvMat*)Real, cvPoint( (Width-1)/2, (Width-1)/2));
		cvFilter2D( (CvMat*)mat, (CvMat*)imat, (CvMat*)Imag, cvPoint( (Width-1)/2, (Width-1)/2));

		cvPow(rmat,rmat,2); 
		cvPow(imat,imat,2);
		cvAdd(imat,rmat,mat); 
		cvPow(mat,mat,0.5); 
		break;
	case CV_GABOR_PHASE:
		break;
	}

	if (dst->depth == IPL_DEPTH_8U)
	{
		cvNormalize((CvMat*)mat, (CvMat*)mat, 0, 255, CV_MINMAX);
		for (int i = 0; i < mat->rows; i++)
		{
			for (int j = 0; j < mat->cols; j++)
			{
				ve = CV_MAT_ELEM(*mat, float, i, j);
				CV_IMAGE_ELEM(dst, uchar, j, i) = (uchar)cvRound(ve);
			}
		}
	}

	if (dst->depth == IPL_DEPTH_32F)
	{
		for (int i = 0; i < mat->rows; i++)
		{
			for (int j = 0; j < mat->cols; j++)
			{
				ve = cvGetReal2D((CvMat*)mat, i, j);
				cvSetReal2D( (IplImage*)dst, j, i, ve );
			}
		}
	}
	cvReleaseMat(&imat);
	cvReleaseMat(&rmat);
	cvReleaseMat(&mat);
}