//
//  main.cpp
//  dlib_test
//
//  Created by 朱瑞 on 17/7/8.
//  Copyright © 2017年 朱瑞. All rights reserved.
//

#include <iostream>

#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>


using namespace dlib;
using namespace std;

string model_path="/Users/zrain/dlib/dlib/shape_predictor_68_face_landmarks.dat";


int main()
{
    try
    {
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }
        cout<<"video id opened!"<<endl;
        //image_window win;
        
        // Load face detection and pose estimation models.
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize(model_path) >> pose_model;
        
        // Grab and process frames until the main window is closed by the user.
        int num=0;
        while (cv::waitKey(30) != 27)
        {
            // Grab a frame
            cv::Mat temp(cv::Size(500,600),CV_8SC3);
            cap >> temp;
            temp.resize(500,500);
            
            cv_image<bgr_pixel> cimg(temp);
            // Detect faces
            std::vector<rectangle> faces = detector(cimg);
            // Find the pose of each face.
            std::vector<full_object_detection> shapes;
            for (unsigned long i = 0; i < faces.size(); ++i)
            {
                if(i==0)
                cout<<"total faces is:"<<num++<<endl;
                shapes.push_back(pose_model(cimg, faces[i]));
            }
            
            if (!shapes.empty()) {
                for (int i = 0; i < 68; i++) {
                    circle(temp, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 3, cv::Scalar(0, 0, 255), -1);
                    //  shapes[0].part(i).x();//68个
                }
            }
            //Display it all on the screen
            cv::namedWindow("test!",CV_WINDOW_NORMAL);
            cv::imshow("test!", temp);
            
        }
    }
    catch (serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch (exception& e)
    {
        cout << e.what() << endl;
    }  
}
