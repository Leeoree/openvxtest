//@file demo_FindContours.h
//@brief Contains demonstration of Threshold function in comparing with OpenCV
//@author Ivan Nankevich
//@date 23 April 2021

#include "../stdafx.h"

#include <opencv2/opencv.hpp>

extern "C"
{
#include <Kernels/ref.h>
#include <types.h>
}

#include "../DemoEngine.h"

///////////////////////////////////////////////////////////////////////////////
//@brief Demonstration of FindCountours function
class demo_FindContours : public IDemoCase
{
public:
    ///@brief default ctor
    demo_FindContours()
        : m_threshold(127)
    {
        // nothing to do
    }

    ///@see IDemoCase::ReplyName
    virtual std::string ReplyName() const override
    {
        return "FindContours";
    }

private:
    ///@see IDemoCase::execute
    virtual void execute() override;

    ///@brief provide interactive demo
    static void applyParameters(int pos, void* data);

private:
    int m_threshold;
    cv::Mat m_srcImage;
};

///////////////////////////////////////////////////////////////////////////////
namespace
{
    const std::string m_openVXWindow = "openVX";
    const std::string m_openCVWindow = "openCV";
    const std::string m_originalWindow = "original";
    const std::string m_diffWindow = "Diff of " + m_openVXWindow + " and " + m_openCVWindow;
}

///////////////////////////////////////////////////////////////////////////////
void demo_FindContours::execute()
{
    cv::namedWindow(m_originalWindow, CV_WINDOW_NORMAL);
    cv::namedWindow(m_openVXWindow, CV_WINDOW_NORMAL);
    cv::namedWindow(m_openCVWindow, CV_WINDOW_NORMAL);
    //cv::namedWindow(m_diffWindow, CV_WINDOW_NORMAL);

    const std::string imgPath = "../../../../Image/testimg1_8UC1.png";
    m_srcImage = cv::imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE);
    cv::imshow(m_originalWindow, m_srcImage);

    cv::createTrackbar("Threshold:", m_originalWindow, &m_threshold, 255, applyParameters, static_cast<void*>(this));
    applyParameters(m_threshold, this);

    cv::waitKey(0);
}

///////////////////////////////////////////////////////////////////////////////
void demo_FindContours::applyParameters(int, void* data)
{
    auto demo = static_cast<demo_FindContours*>(data);

    const cv::Size imgSize(demo->m_srcImage.cols, demo->m_srcImage.rows);

    ///@{ OPENCV
    cv::RNG rng(12345);

    cv::Mat cvImage;
    cv::threshold(demo->m_srcImage, cvImage, demo->m_threshold, 255, CV_THRESH_BINARY);
    
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(cvImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros(cvImage.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        cv::drawContours(drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0);
    }

    cv::imshow(m_openCVWindow, drawing);
    ///@}

    ///@{ OPENVX
    _vx_image srcVXImage = {
       demo->m_srcImage.data,
       uint32_t(imgSize.width),
       uint32_t(imgSize.height),
       VX_DF_IMAGE_U8,
       VX_COLOR_SPACE_DEFAULT
    };

    uint8_t* outVXImage = static_cast<uint8_t*>(calloc(imgSize.width * imgSize.height, sizeof(uint8_t)));
    _vx_image dstVXImage = {
       outVXImage,
       uint32_t(imgSize.width),
       uint32_t(imgSize.height),
       VX_DF_IMAGE_U8,
       VX_COLOR_SPACE_DEFAULT
    };

    ref_FindContours(&srcVXImage, &dstVXImage);

    const cv::Mat vxImage = cv::Mat(imgSize, CV_8UC1, outVXImage);
    cv::imshow(m_openVXWindow, vxImage);
    ///@}

    // Show difference of OpenVX and OpenCV
    /*const cv::Mat diffImage(imgSize, CV_8UC1);
    cv::absdiff(vxImage, cvImage, diffImage);
    cv::imshow(m_diffWindow, diffImage);*/
}

///////////////////////////////////////////////////////////////////////////////
IDemoCasePtr CreateFindContoursDemo()
{
    return std::make_unique<demo_FindContours>();
}
