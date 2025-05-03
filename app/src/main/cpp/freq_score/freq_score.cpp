//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// freq_score.cpp
//
// Code generation for function 'freq_score'
//

// Include files
#include "freq_score.h"
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Declarations
static double compute_power_ratio(const coder::array<double, 2U> &sig,
                                  double fs);

static double freq_score_anonFcn1(const coder::array<double, 2U> &signals,
                                  double fs, double x);

static double rt_hypotd_snf(double u0, double u1);

// Function Definitions
static double compute_power_ratio(const coder::array<double, 2U> &sig,
                                  double fs)
{
  static const double window[256]{0.0,
                                  0.00015177401106419852,
                                  0.00060700390285500783,
                                  0.0013654133071059893,
                                  0.0024265417964677849,
                                  0.0037897451640321322,
                                  0.0054541958144270208,
                                  0.0074188832662467341,
                                  0.0096826147655111328,
                                  0.0122440160097817,
                                  0.015101531982495253,
                                  0.018253427897008456,
                                  0.02169779024977958,
                                  0.025432527982049369,
                                  0.029455373749314229,
                                  0.033763885297822094,
                                  0.038355446947255289,
                                  0.043227271178699567,
                                  0.048376400326935576,
                                  0.0537997083760261,
                                  0.059493902857107761,
                                  0.065455526847235823,
                                  0.071680961068068649,
                                  0.0781664260831168,
                                  0.08490798459222465,
                                  0.091901543821889142,
                                  0.099142858009966639,
                                  0.10662753098325839,
                                  0.11435101882640963,
                                  0.12230863264050312,
                                  0.13049554138967046,
                                  0.13890677483399533,
                                  0.14753722654692641,
                                  0.15638165701536866,
                                  0.16543469682057088,
                                  0.17469084989787897,
                                  0.18414449687337447,
                                  0.19378989847537503,
                                  0.20362119901872255,
                                  0.21363242995974741,
                                  0.2238175135197471,
                                  0.23417026637478194,
                                  0.2446844034095465,
                                  0.25535354153303824,
                                  0.2661712035537066,
                                  0.27713082211173085,
                                  0.28822574366603781,
                                  0.29944923253364064,
                                  0.31079447497884483,
                                  0.32225458334984103,
                                  0.33382260026017019,
                                  0.34549150281252627,
                                  0.35725420686232889,
                                  0.36910357131847987,
                                  0.38103240247869058,
                                  0.3930334583967513,
                                  0.40509945327908703,
                                  0.417223061907935,
                                  0.42939692408845426,
                                  0.44161364911707185,
                                  0.453865820268349,
                                  0.46614599929764622,
                                  0.47844673095685214,
                                  0.490760547520435,
                                  0.50307997331906928,
                                  0.51539752927808513,
                                  0.527705737457985,
                                  0.53999712559427082,
                                  0.55226423163382665,
                                  0.56449960826510148,
                                  0.57669582743934278,
                                  0.58884548488013433,
                                  0.600941204578505,
                                  0.61297564327087373,
                                  0.62494149489711548,
                                  0.63683149503604142,
                                  0.64863842531560134,
                                  0.66035511779512746,
                                  0.67197445931696409,
                                  0.683489395824836,
                                  0.69489293664633967,
                                  0.70617815873695167,
                                  0.71733821088298244,
                                  0.72836631786092032,
                                  0.73925578455064322,
                                  0.74999999999999989,
                                  0.76059244143829252,
                                  0.77102667823622473,
                                  0.78129637580991163,
                                  0.79139529946658049,
                                  0.80131731818962815,
                                  0.81105640836073689,
                                  0.82060665741678906,
                                  0.82996226743936119,
                                  0.839117558674617,
                                  0.84806697298146327,
                                  0.85680507720587618,
                                  0.86532656647934658,
                                  0.87362626743944549,
                                  0.8816991413705515,
                                  0.88954028726283529,
                                  0.89714494478764306,
                                  0.90450849718747373,
                                  0.9116264740787936,
                                  0.9184945541659888,
                                  0.925108567864807,
                                  0.93146449983369484,
                                  0.93755849141149628,
                                  0.943386842960031,
                                  0.94894601611012908,
                                  0.95423263590976193,
                                  0.95924349287296151,
                                  0.96397554492828719,
                                  0.96842591926565524,
                                  0.97259191408040979,
                                  0.97647100021357824,
                                  0.980060822687314,
                                  0.98335920213459371,
                                  0.98636413612230234,
                                  0.98907380036690284,
                                  0.991486549841951,
                                  0.99360091977678455,
                                  0.99541562654578009,
                                  0.9969295684476368,
                                  0.99814182637421478,
                                  0.99905166436852211,
                                  0.99965853007151151,
                                  0.99996205505741531,
                                  0.99996205505741531,
                                  0.99965853007151151,
                                  0.99905166436852211,
                                  0.99814182637421478,
                                  0.9969295684476368,
                                  0.99541562654578009,
                                  0.99360091977678455,
                                  0.991486549841951,
                                  0.98907380036690284,
                                  0.98636413612230234,
                                  0.98335920213459371,
                                  0.980060822687314,
                                  0.97647100021357824,
                                  0.97259191408040979,
                                  0.96842591926565524,
                                  0.96397554492828719,
                                  0.95924349287296151,
                                  0.95423263590976193,
                                  0.94894601611012908,
                                  0.943386842960031,
                                  0.93755849141149628,
                                  0.93146449983369484,
                                  0.925108567864807,
                                  0.9184945541659888,
                                  0.9116264740787936,
                                  0.90450849718747373,
                                  0.89714494478764306,
                                  0.88954028726283529,
                                  0.8816991413705515,
                                  0.87362626743944549,
                                  0.86532656647934658,
                                  0.85680507720587618,
                                  0.84806697298146327,
                                  0.839117558674617,
                                  0.82996226743936119,
                                  0.82060665741678906,
                                  0.81105640836073689,
                                  0.80131731818962815,
                                  0.79139529946658049,
                                  0.78129637580991163,
                                  0.77102667823622473,
                                  0.76059244143829252,
                                  0.74999999999999989,
                                  0.73925578455064322,
                                  0.72836631786092032,
                                  0.71733821088298244,
                                  0.70617815873695167,
                                  0.69489293664633967,
                                  0.683489395824836,
                                  0.67197445931696409,
                                  0.66035511779512746,
                                  0.64863842531560134,
                                  0.63683149503604142,
                                  0.62494149489711548,
                                  0.61297564327087373,
                                  0.600941204578505,
                                  0.58884548488013433,
                                  0.57669582743934278,
                                  0.56449960826510148,
                                  0.55226423163382665,
                                  0.53999712559427082,
                                  0.527705737457985,
                                  0.51539752927808513,
                                  0.50307997331906928,
                                  0.490760547520435,
                                  0.47844673095685214,
                                  0.46614599929764622,
                                  0.453865820268349,
                                  0.44161364911707185,
                                  0.42939692408845426,
                                  0.417223061907935,
                                  0.40509945327908703,
                                  0.3930334583967513,
                                  0.38103240247869058,
                                  0.36910357131847987,
                                  0.35725420686232889,
                                  0.34549150281252627,
                                  0.33382260026017019,
                                  0.32225458334984103,
                                  0.31079447497884483,
                                  0.29944923253364064,
                                  0.28822574366603781,
                                  0.27713082211173085,
                                  0.2661712035537066,
                                  0.25535354153303824,
                                  0.2446844034095465,
                                  0.23417026637478194,
                                  0.2238175135197471,
                                  0.21363242995974741,
                                  0.20362119901872255,
                                  0.19378989847537503,
                                  0.18414449687337447,
                                  0.17469084989787897,
                                  0.16543469682057088,
                                  0.15638165701536866,
                                  0.14753722654692641,
                                  0.13890677483399533,
                                  0.13049554138967046,
                                  0.12230863264050312,
                                  0.11435101882640963,
                                  0.10662753098325839,
                                  0.099142858009966639,
                                  0.091901543821889142,
                                  0.08490798459222465,
                                  0.0781664260831168,
                                  0.071680961068068649,
                                  0.065455526847235823,
                                  0.059493902857107761,
                                  0.0537997083760261,
                                  0.048376400326935576,
                                  0.043227271178699567,
                                  0.038355446947255289,
                                  0.033763885297822094,
                                  0.029455373749314229,
                                  0.025432527982049369,
                                  0.02169779024977958,
                                  0.018253427897008456,
                                  0.015101531982495253,
                                  0.0122440160097817,
                                  0.0096826147655111328,
                                  0.0074188832662467341,
                                  0.0054541958144270208,
                                  0.0037897451640321322,
                                  0.0024265417964677849,
                                  0.0013654133071059893,
                                  0.00060700390285500783,
                                  0.00015177401106419852,
                                  0.0};
  coder::array<double, 2U> welch_segments;
  coder::array<double, 1U> x;
  coder::array<double, 1U> x_detrended;
  coder::array<int, 2U> y;
  double bsum[256];
  double pxx[256];
  double b;
  double b_index;
  double d;
  double segsum;
  int K;
  int lastBlockLength;
  int n;
  int xblockoffset;
  int xj;
  int xoffset;
  int yk;
  unsigned char b_tmp_data[129];
  unsigned char tmp_data[129];
  //  使用Welch方法计算功率谱密度
  //  pwelch 调用
  //  使用 Hann 窗
  //  重叠 128 点
  //  FFT 点数为 256
  x.set_size(sig.size(1));
  n = sig.size(1);
  for (xj = 0; xj < n; xj++) {
    x[xj] = sig[xj];
  }
  //  输入参数说明:
  //  x - 输入信号
  //  window - 窗函数
  //  noverlap - 重叠的样本数
  //  nfft - FFT的点数
  //  fs - 采样频率
  //  确保输入是列向量
  //  如果window是数字，创建一个窗函数
  //  初始化变量
  //  信号总长度
  K = static_cast<int>(
      std::floor((static_cast<double>(x.size(0)) - 128.0) / 128.0));
  //  计算可以分出多少段
  welch_segments.set_size(256, K);
  //  用于存储每个段的FFT结果
  //  分段处理信号
  b_index = 1.0;
  if (K - 1 >= 0) {
    d = 256.0 * fs;
  }
  for (int k{0}; k < K; k++) {
    creal_T X_k[256];
    //  选取当前段
    if (b_index > (b_index + 256.0) - 1.0) {
      xj = 0;
      n = 0;
    } else {
      xj = static_cast<int>(b_index) - 1;
      n = static_cast<int>(static_cast<unsigned int>(b_index) + 255U);
    }
    //  去除线性趋势
    lastBlockLength = n - xj;
    xblockoffset = lastBlockLength - 1;
    if (lastBlockLength - 1 < 0) {
      n = 0;
    } else {
      n = lastBlockLength;
    }
    y.set_size(1, n);
    if (n > 0) {
      yk = 0;
      for (xoffset = 2; xoffset <= n; xoffset++) {
        yk++;
        y[xoffset - 1] = yk;
      }
    }
    if (lastBlockLength == 0) {
      x_detrended.set_size(0);
    } else if (lastBlockLength == 1) {
      x_detrended.set_size(1);
      x_detrended[0] = x[xj] * 0.0;
    } else {
      n = y.size(1);
      segsum = 0.0;
      for (xoffset = 1; xoffset <= n; xoffset++) {
        segsum += x[(xj + xoffset) - 1];
      }
      b = segsum / static_cast<double>(y.size(1));
      x_detrended.set_size(lastBlockLength);
      if (lastBlockLength != 0) {
        n = (lastBlockLength != 1);
        for (xoffset = 0; xoffset <= xblockoffset; xoffset++) {
          x_detrended[xoffset] = x[xj + n * xoffset] - b;
        }
      }
    }
    //  加窗
    //  计算FFT
    if (x_detrended.size(0) == 256) {
      for (xj = 0; xj < 256; xj++) {
        bsum[xj] = x_detrended[xj] * window[xj];
      }
      coder::internal::fft::FFTImplementationCallback::doHalfLengthRadix2(bsum,
                                                                          X_k);
    } else {
      binary_expand_op(x_detrended, window, X_k);
    }
    //  存储功率谱（周期图）
    for (xoffset = 0; xoffset < 256; xoffset++) {
      segsum = rt_hypotd_snf(X_k[xoffset].re, X_k[xoffset].im);
      welch_segments[xoffset + 256 * k] = segsum * segsum / d;
    }
    //  更新索引
    b_index += 128.0;
  }
  //  计算平均功率谱
  if (welch_segments.size(1) == 0) {
    std::memset(&pxx[0], 0, 256U * sizeof(double));
  } else {
    if (welch_segments.size(1) <= 1024) {
      yk = welch_segments.size(1);
      lastBlockLength = 0;
      n = 1;
    } else {
      yk = 1024;
      n = static_cast<int>(static_cast<unsigned int>(welch_segments.size(1)) >>
                           10);
      lastBlockLength = welch_segments.size(1) - (n << 10);
      if (lastBlockLength > 0) {
        n++;
      } else {
        lastBlockLength = 1024;
      }
    }
    for (xj = 0; xj < 256; xj++) {
      pxx[xj] = welch_segments[xj];
    }
    for (int k{2}; k <= yk; k++) {
      xoffset = (k - 1) << 8;
      for (xj = 0; xj < 256; xj++) {
        pxx[xj] += welch_segments[xoffset + xj];
      }
    }
    for (yk = 2; yk <= n; yk++) {
      xblockoffset = (yk - 1) << 18;
      for (xj = 0; xj < 256; xj++) {
        bsum[xj] = welch_segments[xblockoffset + xj];
      }
      if (yk == n) {
        K = lastBlockLength;
      } else {
        K = 1024;
      }
      for (int k{2}; k <= K; k++) {
        xoffset = xblockoffset + ((k - 1) << 8);
        for (xj = 0; xj < 256; xj++) {
          bsum[xj] += welch_segments[xoffset + xj];
        }
      }
      for (xj = 0; xj < 256; xj++) {
        pxx[xj] += bsum[xj];
      }
    }
  }
  for (xj = 0; xj < 256; xj++) {
    pxx[xj] /= static_cast<double>(welch_segments.size(1));
  }
  //  只取一半数据（正频率部分）
  //  生成频率向量
  segsum = fs / 2.0;
  //  pwelch_detrend要求接收列向量
  //  找到0.5Hz到5Hz的频率范围内的功率
  //  计算总功率的频率范围
  xblockoffset = 0;
  n = 0;
  K = 0;
  yk = 0;
  for (lastBlockLength = 0; lastBlockLength < 129; lastBlockLength++) {
    d = segsum * (0.0078125 * static_cast<double>(lastBlockLength));
    if ((d >= 0.5) && (d <= 5.0)) {
      xblockoffset++;
      tmp_data[n] = static_cast<unsigned char>(lastBlockLength);
      n++;
    }
    if (d >= 0.2) {
      K++;
      b_tmp_data[yk] = static_cast<unsigned char>(lastBlockLength);
      yk++;
    }
  }
  //  计算功率占比
  if (xblockoffset == 0) {
    segsum = 0.0;
  } else {
    segsum = pxx[tmp_data[0]];
    for (int k{2}; k <= xblockoffset; k++) {
      segsum += pxx[tmp_data[k - 1]];
    }
  }
  if (K == 0) {
    b = 0.0;
  } else {
    b = pxx[b_tmp_data[0]];
    for (int k{2}; k <= K; k++) {
      b += pxx[b_tmp_data[k - 1]];
    }
  }
  return segsum / b;
}

static double freq_score_anonFcn1(const coder::array<double, 2U> &signals,
                                  double fs, double x)
{
  coder::array<double, 2U> b_signals;
  double b_x;
  int half;
  int i;
  int i1;
  //  将信号分为前后两半
  half =
      static_cast<int>(std::floor(static_cast<double>(signals.size(1)) / 2.0));
  if (half + 1 > signals.size(1)) {
    i = 0;
    i1 = 0;
  } else {
    i = half;
    i1 = signals.size(1);
  }
  //  计算前一半和后一半信号的功率占比
  b_signals.set_size(1, half);
  for (int i2{0}; i2 < half; i2++) {
    b_signals[i2] = signals[(static_cast<int>(x) + signals.size(0) * i2) - 1];
  }
  b_x = compute_power_ratio(b_signals, fs);
  half = i1 - i;
  b_signals.set_size(1, half);
  for (i1 = 0; i1 < half; i1++) {
    b_signals[i1] =
        signals[(static_cast<int>(x) + signals.size(0) * (i + i1)) - 1];
  }
  double y;
  y = compute_power_ratio(b_signals, fs);
  //  返回两个占比中的最小值
  return std::fmin(b_x, y);
}

static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double b;
  double y;
  a = std::abs(u0);
  b = std::abs(u1);
  if (a < b) {
    a /= b;
    y = b * std::sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * std::sqrt(b * b + 1.0);
  } else if (std::isnan(b)) {
    y = rtNaN;
  } else {
    y = a * 1.4142135623730951;
  }
  return y;
}

void freq_score(const coder::array<double, 2U> &signals, double fs,
                double *final_score, double total_socres_data[],
                int total_socres_size[2])
{
  double inputExamples_idx_0;
  int i;
  int idx;
  int k;
  int last;
  signed char y_data[4];
  //  初始化scores数组
  //  计算每个信号的最小功率占比
  if (signals.size(0) < 1) {
    last = 0;
  } else {
    last = signals.size(0);
    idx = signals.size(0) - 1;
    for (i = 0; i <= idx; i++) {
      y_data[i] = static_cast<signed char>(i + 1);
    }
  }
  if (last == 0) {
    inputExamples_idx_0 = 0.0;
  } else {
    inputExamples_idx_0 = y_data[0];
  }
  freq_score_anonFcn1(signals, fs, inputExamples_idx_0);
  total_socres_size[0] = 1;
  total_socres_size[1] = last;
  for (k = 0; k < last; k++) {
    inputExamples_idx_0 =
        freq_score_anonFcn1(signals, fs, static_cast<double>(y_data[k]));
    total_socres_data[k] = inputExamples_idx_0;
  }
  //  计算最大分数
  if (last <= 2) {
    if (last == 1) {
      *final_score = total_socres_data[0];
    } else {
      *final_score = total_socres_data[last - 1];
      if ((!(total_socres_data[0] < *final_score)) &&
          ((!std::isnan(total_socres_data[0])) || std::isnan(*final_score))) {
        *final_score = total_socres_data[0];
      }
    }
  } else {
    if (!std::isnan(total_socres_data[0])) {
      idx = 1;
    } else {
      boolean_T exitg1;
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= last)) {
        if (!std::isnan(total_socres_data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (idx == 0) {
      *final_score = total_socres_data[0];
    } else {
      *final_score = total_socres_data[idx - 1];
      i = idx + 1;
      for (k = i; k <= last; k++) {
        inputExamples_idx_0 = total_socres_data[k - 1];
        if (*final_score < inputExamples_idx_0) {
          *final_score = inputExamples_idx_0;
        }
      }
    }
  }
}

// End of code generation (freq_score.cpp)
