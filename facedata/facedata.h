#ifndef FACEDATA_H
#define FACEDATA_H

#include <matio.h>
#include <iostream>
#include <opencv2/opencv.hpp>


class FaceData
{
public:

    void load_data(mat_t *matfp, int num_pc);
    void load_data(const char *mat_file_name, int num_pc);

    // export face model as a .obj file
    void export_obj(const char *objname);

    // triangle list
    const cv::Mat& tl() const;

    // mean value
    const cv::Mat& mu_shape() const;
    const cv::Mat& mu_tex() const;
    const cv::Mat& mu_exp() const;

    // principal components
    const cv::Mat& pc_shape() const;
    const cv::Mat& pc_tex() const;
    const cv::Mat& pc_exp() const;

    // standard deviation of each principal components
    const cv::Mat& ev_shape() const;
    const cv::Mat& ev_tex() const;
    const cv::Mat& ev_exp() const;

protected:

    cv::Mat m_tl;
    cv::Mat m_mu_shape;
    cv::Mat m_mu_tex;
    cv::Mat m_mu_exp;
    cv::Mat m_pc_shape;
    cv::Mat m_pc_tex;
    cv::Mat m_pc_exp;
    cv::Mat m_ev_shape;
    cv::Mat m_ev_tex;
    cv::Mat m_ev_exp;

};

#endif // FACEDATA_H
