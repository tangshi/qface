#include <fstream>
#include <cstdint>
#include <cstdio>
#include "facedata.h"
#include "obj.h"

using namespace std;
using namespace cv;


void FaceData::load_data(const char *mat_file_name, int num_pc) {
    mat_t *mat = Mat_Open(mat_file_name, MAT_ACC_RDONLY);
    if (mat == nullptr) {
        cerr << "Error: couldn't open mat file " << mat_file_name << endl;
        exit(-1);
    }
    else {
        load_data(mat, num_pc);
        Mat_Close(mat);
    }
}

void FaceData::load_data(mat_t *matfp, int num_pc) {

    auto read_data = [matfp] (Mat &cvmat, const char *varname, int data_type, size_t max_cols) {

        matvar_t *matvar = Mat_VarReadInfo(matfp, varname);

        size_t rows, cols;

        rows = matvar->dims[0];

        if (max_cols > 0 && matvar->dims[1] > max_cols) {
            cols = max_cols;
        }
        else {
            cols = matvar->dims[1];
        }

        cvmat.create(rows, cols, data_type);
        Mat_VarReadDataLinear(matfp, matvar, cvmat.data, 0, 1, rows * cols);

        Mat_VarFree(matvar);

    };


    read_data(m_tl,       "tl",       CV_32SC1, 0);

    read_data(m_mu_exp,   "mu_exp",   CV_64FC1, 0);
    read_data(m_mu_shape, "mu_shape", CV_32FC1, 0);
    read_data(m_mu_tex,   "mu_tex",   CV_32FC1, 0);

    read_data(m_ev_exp,   "ev_exp",   CV_64FC1, 0);
    read_data(m_ev_shape, "ev_shape", CV_32FC1, 0);
    read_data(m_ev_tex,   "ev_tex",   CV_32FC1, 0);

    read_data(m_pc_exp,   "pc_exp",   CV_64FC1, num_pc);
    read_data(m_pc_shape, "pc_shape", CV_32FC1, num_pc);
    read_data(m_pc_tex,   "pc_tex",   CV_32FC1, num_pc);

}

void FaceData::export_obj(const char *objname) {

    float *vertices_ptr = (float*)(m_mu_shape.data);
    int   *tl_ptr       = (int*)(m_tl.data);

    obj *mean_obj = obj_create(NULL);

    for (int i=0; i<m_mu_shape.rows; i = i + 3) {
        int vi = obj_add_vert(mean_obj);
        obj_set_vert_v(mean_obj, vi, vertices_ptr + i);
    }

    int si = obj_add_surf(mean_obj);
    for (int i=0; i<m_tl.cols; ++i) {
        int pi = obj_add_poly(mean_obj, si);
        obj_set_poly(mean_obj, si, pi, tl_ptr + 3*i);
    }

    obj_write(mean_obj, objname, NULL, 6);
}


const cv::Mat& FaceData::tl() const {
    return m_tl;
}

const cv::Mat& FaceData::mu_shape() const {
    return m_mu_shape;
}

const cv::Mat& FaceData::mu_tex() const {
    return m_mu_tex;
}

const cv::Mat& FaceData::mu_exp() const {
    return m_mu_exp;
}

const cv::Mat& FaceData::pc_shape() const {
    return m_pc_shape;
}

const cv::Mat& FaceData::pc_tex() const {
    return m_pc_tex;
}

const cv::Mat& FaceData::pc_exp() const {
    return m_pc_exp;
}

const cv::Mat& FaceData::ev_shape() const {
    return m_ev_shape;
}

const cv::Mat& FaceData::ev_tex() const {
    return m_ev_tex;
}

const cv::Mat& FaceData::ev_exp() const {
    return m_ev_exp;
}
