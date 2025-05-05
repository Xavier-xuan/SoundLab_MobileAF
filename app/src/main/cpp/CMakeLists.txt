cmake_minimum_required(VERSION 3.22.1)

project("soundlab")

# =========================b
# 1. 查找源文件
# =========================

# freq_score 模块
file(GLOB FREQ_SCORE_SRC
        "freq_score/*.cpp"
)

# corr_score 模块
file(GLOB CORR_SCORE_SRC
        "corr_score/*.cpp"
)

# filtfilt 模块
file(GLOB FILTFILT_SRC
        "FiltfiltOrderThreeButter/*.cpp"
)

# 打印 GLOB 匹配到的源文件列表
message(STATUS "Filtfilt source files:")
foreach(f ${FILTFILT_SRC})
    message(STATUS " - ${f}")
endforeach()

# JNI 桥接层
set(JNI_SRC
        soundlab.cpp
)

# =========================
# 2. 添加静态库 freqscore
# =========================
add_library(freqscore STATIC ${FREQ_SCORE_SRC})
target_include_directories(freqscore PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/freq_score
)

# =========================
# 3. 添加静态库 corrscore
# =========================
add_library(corrscore STATIC ${CORR_SCORE_SRC})
target_include_directories(corrscore PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/corr_score
)

# =========================
# 4. 添加静态库 filtfiltorderthreebutter
# =========================
add_library(filtfiltorderthreebutter STATIC ${FILTFILT_SRC})
target_include_directories(filtfiltorderthreebutter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/FiltfiltOrderThreeButter
)

# =========================
# 5. 添加主 JNI 共享库 soundlab
# =========================
add_library(${CMAKE_PROJECT_NAME} SHARED
        ${JNI_SRC}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/freq_score
        ${CMAKE_CURRENT_SOURCE_DIR}/corr_score
        ${CMAKE_CURRENT_SOURCE_DIR}/FiltfiltOrderThreeButter

)

# =========================
# 6. 链接依赖库
# =========================
target_link_libraries(${CMAKE_PROJECT_NAME}
        freqscore
        corrscore
        filtfiltorderthreebutter
        android
        log
        m
)
