#include "../../../_common/vOpenCV/OpenCV.h"

VideoInput input;

using namespace cv;

int main(int argc, char** argv )
{	
	if (input.init(argc,argv))
	{
		while (true)
		{
			IplImage* _raw = input.get_frame(); 
			if (!_raw)
				break;
			Mat raw = _raw;
			show_mat(raw);
			
			int key = waitKey(1);
			if (key == 0x1B)
				break;
		}
	}

	return 0;
}
