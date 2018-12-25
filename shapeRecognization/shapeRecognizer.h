#pragma once

#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "stdio.h"

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <string>
using  namespace   std;

#include <opencv2/opencv.hpp>
using  namespace   cv;


class shapeRecognizer
{
public:
	shapeRecognizer();
	~shapeRecognizer();

public:

	int  get_core_radius_of_contour(vector<Point> input,
		Point2f  &core, double  & radius,
		double & sideMax, double & sideMin,
		vector<double >&  sideLength
	)
	{
		if (input.size() == 0)
			return -1;

		//vector<double >  sideLength;

		double x = 0;
		double  y = 0;
		double r = 0;
		for (int i = 0; i < input.size(); i++)
		{
			x += input[i].x;
			y += input[i].y;

			double sideLen = 0;
			if (i > 0)
				sideLen = sqrt(pow(input[i].x - input[i - 1].x, 2.0) + pow(input[i].y - input[i - 1].y, 2.0));
			else
				sideLen = sqrt(pow(input[i].x - input[input.size() - 1].x, 2.0) + pow(input[i].y - input[input.size() - 1].y, 2.0));
			sideLength.push_back(sideLen);
		}
		x = x / input.size();
		y = y / input.size();
		core.x = x;
		core.y = y;

		sideMax = sideLength[0];
		sideMin = sideLength[0];
		for (int i = 0; i < input.size(); i++)
		{
			double r_xy = sqrt(pow(input[i].x - x, 2.0) + pow(input[i].y - y, 2.0));
			r += r_xy;

			if (sideMax < sideLength[i])
				sideMax = sideLength[i];
			if (sideMin > sideLength[i])
				sideMin = sideLength[i];
		}
		radius = r / input.size();

		return 0;
	}


	int  calc_diff_percent_two(vector<Point> input, vector<Point>  contours_poly, Mat srcImage, double  & diff_rate)
	{
		Point2f   core;
		double   radius;
		double sideMax;
		double sideMin;
		vector<double >  sideLength;
		get_core_radius_of_contour(input, core, radius, sideMax, sideMin, sideLength);


		if (input.size() <= 0 || contours_poly.size() <= 0)
			return  -1;

		Mat  drawing(srcImage.size(), CV_8UC1, Scalar(0));
		Mat drawing_po = drawing.clone();

		vector<vector<Point>>   vec_input;
		vec_input.push_back(input);
		drawContours(drawing, vec_input, 0, Scalar(255), -1, 8);
		int  numsCont = countNonZero(drawing);

		vector<vector<Point>>   vec_contours_poly;
		vec_contours_poly.push_back(contours_poly);
		drawContours(drawing_po, vec_contours_poly, 0, Scalar(255), -1, 8);
		int  numsCont_po = countNonZero(drawing_po);

		if (radius == 0)
			diff_rate = 999;
		else
		{
			//diff_rate = (double)(fabs)(numsCont_po - numsCont) / (double)(CV_PI* radius* radius);
			//diff_rate = (double) (numsCont_po - numsCont) / (double)(CV_PI* radius* radius);

			diff_rate = (double)(numsCont_po - numsCont) / (double)(numsCont);
		}

		return  0;
	}

	int  calc_mean_variance(vector<double >  sideLength, double & mean, double & var, double radius)
	{
		if (sideLength.size() <= 0)
			return  -1;
		if (radius == 0)
			return  -1;

		mean = 0;
		var = 0;
		for (int i = 0; i < sideLength.size(); i++)
		{
			mean += sideLength[i];
		}
		mean = mean / sideLength.size();

		for (int i = 0; i < sideLength.size(); i++)
		{
			var += pow(sideLength[i] - mean, 2.0);
		}
		var = var / sideLength.size() / radius;

		return  0;
	}

	// 最佳正多边形的逼近
	//best  approximation   accuracy
	int   min_error_approximation(
		vector<Point> input, Mat srcImage,
		double    epsilonInf, double epsilonSup, int  numRef,
		int & k, vector<Point> &  contours_poly,
		const double  rate_error_control_Inf, double  rate_error_control_Sup,
		double & rate_error__approMinusOrigin,
		const int  k_solid, int  numsStep
	)
	{
		//cout << "开始搜索最小方差项同时最小误差逼近(五步搜索)： " << endl;
		//int  k_in = k;

		Point2f   core_origin(0, 0);
		double   radius_origin = 0;
		double sideMax_origin = 0;
		double sideMin_origin = 0;
		vector<double >  sideLength_origin;
		get_core_radius_of_contour(input, core_origin, radius_origin, sideMax_origin, sideMin_origin, sideLength_origin);

		//cout << "epsilonInf ,epsilonSup = " << epsilonInf << " , " << epsilonSup << endl;

		//show 一下
		//Mat  srcImage_show = srcImage.clone();
		//circle(srcImage_show, core_origin, radius_origin, Scalar(0, 255, 0), 1);
		//for (int i = 0; i < input.size(); i++)
		//{
		//	line(srcImage_show, input[i], core_origin, Scalar(0, 255, 0), 1, 8, 0);
		//}
		//imshow(("srcImage_show_" + to_string(numRef) + ".jpg").c_str(), srcImage_show);
		//waitKey(200);


		const  double   nums_of_get = numsStep;// 25;// 6;// 5;// 3;//;// 15;// 5;// 15;// 15;// 10;// radius_origin * 30;// 100;// radius_origin * 30;// 100;
		const  double  stepLen = (epsilonSup - epsilonInf) / nums_of_get;// radius_origin / nums_of_get;// / 4;// *2;// / nums_of_get;// 10;// scale_shift / nums_of_get;
		double  min_var = 0;
		int  i_special = -1;

		double  evauluation = 0;
		double  radius_special_poly = 0;
		double   diff_rate_special = 0;
		Point2f core_special;
		double epsilon = 0;
		for (double i = 0; i < nums_of_get; i += 1)
		{
			double epsilonCur = 0;
			epsilonCur = epsilonInf + stepLen* i;
			vector<Point>    contours_poly_val;
			//contours_poly_val.clear();
			approxPolyDP(input, contours_poly_val, epsilonCur, true);

			//  指标1
			Point2f   core;
			double   radius;
			double sideMax;
			double sideMin;
			vector<double >  sideLength;
			get_core_radius_of_contour(contours_poly_val, core, radius, sideMax, sideMin, sideLength);
			double   mean;
			double  var;
			calc_mean_variance(sideLength, mean, var, radius);

			//  指标2
			double   diff_rate_polyMinusOrigin = 99;
			int res_per = calc_diff_percent_two(input, contours_poly_val, srcImage, diff_rate_polyMinusOrigin);

			double  evauluation_tmp;
			evauluation_tmp = 1.0 / (pow(var, 2.0) + 1.0) + 1.0 / (pow(diff_rate_polyMinusOrigin, 2.0) + 1.0);
 

			//cout << "( " << i << " , " << contours_poly_val.size() << " ," << epsilonCur / radius_origin << " ," << evauluation_tmp << " ,";
			//cout << var << " ," << diff_rate_polyMinusOrigin << " , " << radius_origin << "), " << endl;
 

			if (contours_poly_val.size() < 3)
				continue;

			if (fabs(diff_rate_polyMinusOrigin) > rate_error_control_Sup)
				continue;

			if (k_solid == 0)
			{
				if (i == 0)
				{
					i_special = i;
					epsilon = epsilonCur;
					min_var = var;

					contours_poly = contours_poly_val;
					k = contours_poly_val.size();
					radius_special_poly = radius;
					diff_rate_special = diff_rate_polyMinusOrigin;
					rate_error__approMinusOrigin = diff_rate_polyMinusOrigin;
					core_special = core;

					evauluation = evauluation_tmp;
				}

				if (evauluation < evauluation_tmp)
				{
					i_special = i;
					epsilon = epsilonCur;
					min_var = var;

					contours_poly = contours_poly_val;
					k = contours_poly_val.size();
					radius_special_poly = radius;
					diff_rate_special = diff_rate_polyMinusOrigin;
					rate_error__approMinusOrigin = diff_rate_polyMinusOrigin;
					core_special = core;

					evauluation = evauluation_tmp;
				}
			}
			else
			{
				if (contours_poly_val.size() == k_solid)
				{
					if (i_special == -1)
					{
						i_special = i;
						epsilon = epsilonCur;
						min_var = var;

						contours_poly = contours_poly_val;
						k = contours_poly_val.size();
						radius_special_poly = radius;
						diff_rate_special = diff_rate_polyMinusOrigin;
						rate_error__approMinusOrigin = diff_rate_polyMinusOrigin;
						core_special = core;

						evauluation = evauluation_tmp;
					}


					if (evauluation < evauluation_tmp)
					{
						i_special = i;
						epsilon = epsilonCur;
						min_var = var;

						contours_poly = contours_poly_val;
						k = contours_poly_val.size();
						radius_special_poly = radius;
						diff_rate_special = diff_rate_polyMinusOrigin;
						rate_error__approMinusOrigin = diff_rate_polyMinusOrigin;
						core_special = core;

						evauluation = evauluation_tmp;
					}
				}
			}
		}

		//cout << "get:    k = " << k << endl;


		////show 
		//Mat  srcImage_show_poly = srcImage.clone();
		//circle(srcImage_show_poly, core_special, radius_special_poly, Scalar(0, 255, 255), 1);
		//for (int i = 0; i < contours_poly.size(); i++)
		//{
		//	line(srcImage_show_poly, contours_poly[i], core_special, Scalar(0, 255, 0), 1, 8, 0);
		//}
		//imshow(("srcImage_show_poly_" + to_string(numRef) + ".jpg").c_str(), srcImage_show_poly);
		////imshow(("srcImage_show_poly.jpg") , srcImage_show_poly);
		//waitKey(200);

		//0.10 是经验数值，避免对圆做逼近的时候出现外接四边形这类高误差率的逼近
		//double  rate_ref = 0.15;
		if (fabs(rate_error__approMinusOrigin) > rate_error_control_Sup)
		{
			return -1;
		}
		else
		{
			return 0;
		}

	}


	int  best_fit__recoItsShape(Mat show_best, vector<Point >input,
		vector<Point > & contours_poly, int & k,
		int  nums_ref,
		const double  rate_error_control_Inf, double  rate_error_control_Sup,
		double & rate_error__appMinusOrigin, int  numsSteps
	)
	{
		if (1)
		{
			Point2f  core_origin;
			double r_origin;
			double sideMax_origin;
			double sideMin_origin;
			vector<double >  sideLength_origin;
			get_core_radius_of_contour(input, core_origin, r_origin, sideMax_origin, sideMin_origin, sideLength_origin);



			const    double  epsilon = r_origin;
			int  res_k = -1;
			int generation = 0;
			//double  rate_ref = rate_error;
			int  numsStep = numsSteps;
			while (1)
			{
				generation++;
				cout << "开始第几轮搜索：generation = " << generation << endl;
				cout << "===================  ===================  ==================" << endl;
				const double epsilonInf = 2;    // max(1.0, r_origin / 20);
				const double epsilonSup = r_origin;
				res_k = min_error_approximation
				(
					input, show_best, epsilonInf, epsilonSup, nums_ref,
					k, contours_poly,
					rate_error_control_Inf, rate_error_control_Sup,
					rate_error__appMinusOrigin, 0, numsStep
				);

				if (abs(rate_error__appMinusOrigin) > rate_error_control_Sup)
					return  -1;
				else
					return  0;
				break;
			}
			if (res_k < 0)
				return -1;
			else
				return  0;
		}
		return  0;
	}


	int  best_fit(Mat show_best, vector<Point >  input,
		vector<Point > & contours_poly, int & k,
		int  nums_ref,
		const double  rate_error_control_Inf, double  rate_error_control_Sup,
		double & rate_error__appMinusOrigin, int  numsSteps
	)
	{
		if (1)
		{
			Point2f  core_origin;
			double r_origin;
			double sideMax_origin;
			double sideMin_origin;
			vector<double >  sideLength_origin;
			get_core_radius_of_contour(input, core_origin, r_origin, sideMax_origin, sideMin_origin, sideLength_origin);



			const    double  epsilon = r_origin;
			int  res_k = -1;
			int generation = 0;
			//double  rate_ref = rate_error;
			int  numsStep = numsSteps;
			while (1)
			{
				generation++;
				cout << "开始第几轮搜索：generation = " << generation << endl;
				cout << "===================  ===================  ==================" << endl;
				const double epsilonInf = max(1.0, r_origin / 20);
				const double epsilonSup = r_origin;
				res_k = min_error_approximation(input, show_best, epsilonInf, epsilonSup, nums_ref,
					k, contours_poly,
					rate_error_control_Inf, rate_error_control_Sup,
					rate_error__appMinusOrigin, 0, numsStep);

				if (abs(rate_error__appMinusOrigin) > rate_error_control_Sup)
					return  -1;
				else
					return  0;

				break;
			}
			if (res_k < 0)
				return -1;
			else
				return  0;
		}
	}





	//最佳指定边数的逼近
	//best const k  approximation   accuracy
	int  best_fit_const_k_drivedByErrorRate(Mat show_best, vector<Point >  input,
		vector<Point > & contours_poly, int &  k,
		int  nums_ref,
		const double  rate_error_control_Inf, double  rate_error_control_Sup,
		double & rate_error__appMinusOrigin,
		int  numsSteps, int  k_control)
	{
		Point2f  core_origin;
		double r_origin;
		double sideMax_origin;
		double sideMin_origin;
		vector<double >  sideLength_origin;
		get_core_radius_of_contour(input, core_origin, r_origin, sideMax_origin, sideMin_origin, sideLength_origin);

		int  generaton_append = 0;
		int  steps = numsSteps;
	double  	rate_epsilon = 3.0;
		while (1)
		{
			generaton_append++;
			double  epsilon = r_origin;

			int  res_findEp;
			double epsilonInf = 1;
			double epsilonSup = r_origin * rate_epsilon;  // 0.4;// 2.0;// 0.5;// 2;

			
			res_findEp = min_error_approximation(
				input, show_best, epsilonInf, epsilonSup, nums_ref,
				k, contours_poly,
				rate_error_control_Inf, rate_error_control_Sup,
				rate_error__appMinusOrigin, k_control, steps);
			cout << "generaton_append ,k , k_control , steps, epsilonSup =  " << generaton_append << " , ";
			cout << k << " , ";
			cout << k_control << " , "; 
			cout << steps << " , ";
			cout << epsilonSup << " , ";
			cout<< endl;

			if (steps > ( int )epsilonSup   )
				break;

			if (k != k_control)
			{
				const  double  rate =  1.05 ;
				steps = steps*  rate;
			}
			else
				break;
		}
		//if (k != k_control)
		//	return  generaton_append;
		return  generaton_append;
	}



	void approxPolyDP_app_fit_4p(int  thresh, Mat  srcImage, vector<Point2f> & pts4, double   epsilon, int  numsRef)
	{
		pts4.clear();

		Mat grayImage;
		if (srcImage.channels() > 1)
		{
			cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
		}
		else
		{
			grayImage = srcImage;
		}
		const int threshMaxValue = 255;
		RNG rng(12345);

		Mat threshold_output;
		vector< vector< Point> > contours;
		vector<Vec4i > hierarchy;

		// 
		threshold(grayImage, threshold_output, thresh, 255, THRESH_BINARY);
		//寻找图像轮廓   
		findContours(threshold_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		cout << "contours.size() = " << contours.size() << endl;

		// 
		vector<vector<Point>>contours_poly(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), contours_poly[i], epsilon, true);
		}

		cout << "contours_poly[0].size() = " << contours_poly[0].size() << endl;
		for (int i = 0; i < contours_poly[0].size(); i++)
		{
			pts4.push_back(Point2f(contours_poly[0][i].x, contours_poly[0][i].y));
		}

		//  
		Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
		char  carNmae[300];
		for (int i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
			for (int j = 0; j < contours_poly[0].size(); j++)
			{
				sprintf(carNmae, "%d", j);
				putText(drawing, carNmae, contours_poly[0][j], 1, 1, Scalar(255, 0, 255));
			}
		}

		imwrite((string("contour_") + to_string(numsRef) + string(".jpg")).c_str(), drawing);
		if (pts4.size() == 4)
		{
			namedWindow(string("contour_") + to_string(numsRef), WINDOW_AUTOSIZE);
			imshow(string("contour_") + to_string(numsRef), drawing);
			waitKey(200);
			//destroyWindow(string("contour_") + to_string(numsRef));
		}

		return;
	}




	int  getContours(Mat  src_t, vector<vector<Point> > &   contours_t, 
		int   opt_function_t, double rateErrorSup, 
		int  numsSteps , int  k_solid ,
		string   nameOfImageIn
		)
	{
		Mat src = src_t.clone();
		if (src.channels() == 3)
		{
			Mat  gray;
			cvtColor(src, gray, CV_BGR2GRAY);
			gray.copyTo(src);
		}

		//滤波
		threshold(src, src, 128, 255, 0);
		GaussianBlur(src, src, Size(5, 5), 3, 3);

		// 轮廓
		vector<vector<Point> >     contours_origin;
		findContours(src, contours_origin, CV_RETR_EXTERNAL, 1);

		//寻找图像凸包
		vector<vector<Point> >  hull(contours_origin.size());
		for (int i = 0; i < contours_origin.size(); i++)
		{
			convexHull(Mat(contours_origin[i]), hull[i], false);
		}
		cout << "contours_origin.size() = " << contours_origin.size() << endl;
		imshow("src_after_gaussian", src);
		cv::waitKey(10);

		vector<vector<Point> >   contours;
		contours = hull;// contours_origin;// hull;
		int  opt_function = opt_function_t; //   1;
		int  generation = -1;

		// 最佳逼近
		Mat show_best = src_t.clone();
		vector<vector<Point>>contours_poly(contours.size());
		vector<int >  vct__generation;
		for (int i = 0; i < contours.size(); i++)
		{
			//if (i != 2)
			//	continue;

			cout << "_________________________________________________________________________________" << endl;
			cout << "轮廓开始处理，编号 i = " << i << endl;
			//任意边数的多边形逼近     0
			if (opt_function == 0)
			{
				double epsilon = 3.0;
				approxPolyDP(Mat(contours[i]), contours_poly[i], epsilon, true);
			}

			int  k = -1;
			//任意边数的多边形逼近     1  
			if (opt_function == 1)
			{
				cout << "任意多边形拟合开始：" << endl;
				const  double  rate_error_control_Inf = 1;// 0.07510;
				double  rate_error_control_Sup = rateErrorSup;// 0.15;// 0.07510;
				double  rate_error__appMinusOrigin = 1.0;
				int  res_bf = best_fit(show_best, contours[i], contours_poly[i], k, i,
					rate_error_control_Inf, rate_error_control_Sup, rate_error__appMinusOrigin, numsSteps);
				cout << "(--) i ,  k,    contours_poly[i].size() ,  rate_error__appMinusOrigin,  contours[i].size() ,  = " << i << " , " << k << " , ";
				cout << contours_poly[i].size() << ",";
				cout << rate_error__appMinusOrigin << " , " << contours[i].size() << endl;
				if (res_bf < 0)
					cout << ">>>>>>>>>>>>>>>>>>>  在该尺度下拟合失败。" << endl;
				else
					cout << "在该尺度下拟合成功。" << endl;
			}


			//指定边数的多边形逼近    2 
			if (opt_function == 2)
			{
				cout << "四边形拟合开始：" << endl;
				k = -1;
				   int   k_control = k_solid   ;//  rate_error = 0.70;
				const  double  rate_error_control_Inf = 0;// 0.07510;
				double  rate_error_control_Sup = rateErrorSup;// 0.15;// 0.07510;
				//const  double  rate_error_control = 0.90;//  0.67
				double  rate_error__appMinusOrigin = 1.0;
				int  steps = numsSteps;// 30;
				generation = best_fit_const_k_drivedByErrorRate(show_best, contours[i], contours_poly[i], k,
					i, rate_error_control_Inf, rate_error_control_Sup, rate_error__appMinusOrigin, steps, k_control);

				vct__generation.push_back(generation);
				//int  generaton_append = 1;
				////int  indic = 0;
				//while (k != k_control   )
				//{
				   // cout << "增补处理第几代？   generaton_append = " << generaton_append << endl;
				   // steps = steps * 2;
				   // generation = best_fit_const_k_drivedByErrorRate(show_best, contours[i], contours_poly[i], k,
					  //  i, rate_error_control_Inf, rate_error_control_Sup, rate_error__appMinusOrigin, steps);
				   // if(steps >    )
				//}

				if (k != k_control)
				{
					cout << "四边形拟合失败。k , k_control = " << k << " , " << k_control << endl;
					continue;
				}
				else
				{
					cout << "四边形拟合成功。k , k_control = " << k <<" , "<< k_control <<  endl;
				}
				cout << "<--> i ,  k,    contours_poly[i].size() ,  rate_error__appMinusOrigin,  contours[i].size() ,  = " << i << " , " << k << " , ";
				cout << contours_poly[i].size() << ",";
				cout << rate_error__appMinusOrigin << " , " << contours[i].size() << endl;
			}

			//认识出它的形状--是几边形的     3  
			//方法：方差var要最小，s.t.  误差在一定范围[alpha,beta]内考虑，即对上界贪婪，也对下界贪婪。
			if (opt_function == 3)
			{
				cout << "任意多边形拟合开始：" << endl;
				const  double  rate_error_control_Inf = 0;      // 0.07510;
				double  rate_error_control_Sup = rateErrorSup;// 0.15;// 0.07510;
				double  rate_error__appMinusOrigin = 1.0;
				int  res_bf = best_fit__recoItsShape(show_best, contours[i], contours_poly[i], k,
					i,
					rate_error_control_Inf, rate_error_control_Sup,
					rate_error__appMinusOrigin, numsSteps);
				cout << "(--) i ,  k,    contours_poly[i].size() ,  rate_error__appMinusOrigin,  contours[i].size() ,  = " << i << " , " << k << " , ";
				cout << contours_poly[i].size() << ",";
				cout << rate_error__appMinusOrigin << " , " << contours[i].size() << endl;
				if (res_bf < 0)
					cout << ">>>>>>>>>>>>>>>>>>>  在该尺度下拟合失败。" << endl;
				else
					cout << "在该尺度下拟合成功。" << endl;
			}

			//最小矩形覆盖       -1
			if (opt_function < 0)
			{
				Mat  画布 = src_t.clone();
				for (int i = 0; i < contours.size(); i++)
				{
					//绘制轮廓  
					drawContours(画布, contours, i, Scalar(255), 1, 8);

					RotatedRect rect = minAreaRect(contours[i]);
					Point2f P[4];

					rect.points(P);
					for (int j = 0; j <= 3; j++)
					{
						//line(原图, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 1);
						line(画布, P[j], P[(j + 1) % 4], Scalar(111), 2);
					}
				}
				imwrite("画布.jpg", 画布);
				imshow("画布", 画布);
				waitKey(20);
			}

			if (opt_function >= 0)
			{

			}
		}



		contours_t = contours_poly;
		if (opt_function >= 0)
		{


			//显示最终结果
			RNG rng(12345);
			Mat  drawing_final = src_t.clone();
			char  carNmae[300];
			for (int i = 0; i < contours_t.size(); i++)
			{
				//if (i != 2)
				//	continue;

				if (contours_poly[i].size() <= 0)
					continue;


				Point2f  core;
				double r;
				double sideMax;
				double sideMin;
				vector<double >  sideLength;
				get_core_radius_of_contour(contours_poly[i], core, r, sideMax, sideMin, sideLength);
				putText(show_best, to_string(i).c_str(), Point(core.x, core.y), 1, 3, Scalar(255, 255, 255));
				putText(show_best, to_string(vct__generation[i]).c_str(), Point(core.x + 18, core.y + 18), 1, 3, Scalar(0, 255, 0));
				circle(show_best, core, 2, Scalar(0, 255, 255), 1, 8, 0);
				circle(show_best, core, r, Scalar(0, 0, 255), 1, 8, 0);



				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				//color = Scalar(  255,  255- 10* i ,    10*i    );
				drawContours(drawing_final, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
				for (int j = 0; j < contours_poly[i].size(); j++)
				{
					sprintf(carNmae, "%d", j);
					putText(drawing_final, carNmae, contours_poly[i][j], 1, 1, Scalar(255, 0, 255));
				}
			}
			imwrite((nameOfImageIn+"show_best.jpg.k_solid." + to_string(k_solid) + ".jpg").c_str(), show_best);
			imshow((nameOfImageIn + "show_best.jpg.k_solid." + to_string(k_solid) + ".jpg").c_str(), show_best);
			cv::waitKey(10);



			imwrite((nameOfImageIn + "drawing_final.jpg.k_solid." + to_string(k_solid) + ".jpg").c_str(), drawing_final);
			imshow((nameOfImageIn + "drawing_final.jpg.k_solid." + to_string(k_solid) + ".jpg").c_str(), drawing_final);
			cv::waitKey(10);
		}
		return 0;
	}
};

