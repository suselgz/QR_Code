#ifndef FBC_ZXING_FUNSET_HPP_
#define FBC_ZXING_FUNSET_HPP_
#include "opencv2/opencv.hpp"
// int test_Aztec_decode();
// int test_DataMatrix_decode();
// int test_Maxicode_decode();
// int test_PDF417_decode();
int test_QRCode_decode(cv::Mat srcImg, std::string& code);
// int test_Codabar_decode();
// int test_Code39_decode();
// int test_Code93_decode();
// int test_Code128_decode();
// int test_EAN8_decode();
// int test_EAN13_decode();
// int test_ITF25_decode();
// int test_RSS14_decode();
// int test_RSS_Expanded_decode();
// int test_UPC_A_decode();
// int test_UPC_E_decode();

#endif // FBC_ZXING_FUNSET_HPP_
