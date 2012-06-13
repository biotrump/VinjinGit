#ifndef _FUSHION_H_
#define _FUSHION_H_

#pragma warning( disable: 4819 ) //���ļ����������ڵ�ǰ����ҳ(936)�б�ʾ���ַ�
#pragma warning( disable: 4373 ) //�麯����д
#pragma warning( disable: 4305 ) //�ӡ�double������float���ض�
#pragma warning( disable: 4181 ) //Ӧ�õ��������͵��޶������ѽ������

#include "../../_common/Kinect/KinectDevice.h"
#include "../../_common/pcl/pcl.h"
#include <boost/thread/thread.hpp>
#include "../../_common/pcl/kinect_support.h"

struct KinectFushionApp : public KinectDevice
{ 
	bool exit;

	KinectFushionApp();

	bool setup();

	void compute_features(PointCloudRgbPtr cloud, PointCloudRgbPtr keypoints, LocalDescriptorsPtr local_descriptors);

	Eigen::Matrix4f initial_alignment(PointCloudRgbPtr src_points, PointCloudRgbPtr dst_points);

	Eigen::Matrix4f icp_alignment(PointCloudRgbPtr src_points, PointCloudRgbPtr dst_points, Eigen::Matrix4f& tform);

	void onDepthData(const cv::Mat& depth_u16);

	PointCloudRgbPtr _cloud;
	PointCloudRgbPtr _cloud_raw;
	PointCloudRgbPtr _cloud_registered;
	PointCloudRgbPtr _model;

	Eigen::Matrix4f tform;

	boost::mutex _mtx;
	CSimpleEvent _evt;

	//OpenGL
	cv::Ptr<PointCloudViewer<pcl::PointXYZRGB> > _raw_viewer;
	cv::Ptr<PointCloudViewer<pcl::PointXYZRGB> > _icp_viewer;
	
	//filter
	int _thresh_near,_thresh_far;
};

#endif //_FUSHION_H_