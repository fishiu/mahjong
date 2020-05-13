cmake_minimum_required(VERSION 3.16)
project()

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(json)
include_directories(MahjongGB)
include_directories(MahjongGB/mahjong-algorithm)
include_directories(reference/C++�齫Դ��)
include_directories(reference/�齫AIԴ����/majiang/majiang)

add_executable(
        json/json-forwards.h
        json/json.h
        MahjongGB/mahjong-algorithm/fan_calculator.cpp
        MahjongGB/mahjong-algorithm/fan_calculator.h
        MahjongGB/mahjong-algorithm/shanten.cpp
        MahjongGB/mahjong-algorithm/shanten.h
        MahjongGB/mahjong-algorithm/standard_tiles.h
        MahjongGB/mahjong-algorithm/stringify.cpp
        MahjongGB/mahjong-algorithm/stringify.h
        MahjongGB/mahjong-algorithm/tile.h
        MahjongGB/mahjong-algorithm/unit_test.cpp
        MahjongGB/MahjongGB.cpp
        MahjongGB/MahjongGB.h
        reference/C++�齫Դ��/CMJ.cpp
        reference/C++�齫Դ��/CMJ.h
        reference/C++�齫Դ��/CMJManage.cpp
        reference/C++�齫Դ��/CMJManage.h
        reference/C++�齫Դ��/lala.cpp
        reference/�齫AIԴ����/majiang/majiang/Cards.cpp
        reference/�齫AIԴ����/majiang/majiang/Cards.h
        reference/�齫AIԴ����/majiang/majiang/Game.cpp
        reference/�齫AIԴ����/majiang/majiang/Game.h
        reference/�齫AIԴ����/majiang/majiang/GameData.cpp
        reference/�齫AIԴ����/majiang/majiang/GameData.h
        reference/�齫AIԴ����/majiang/majiang/MainFrm.cpp
        reference/�齫AIԴ����/majiang/majiang/MainFrm.h
        reference/�齫AIԴ����/majiang/majiang/majiang.cpp
        reference/�齫AIԴ����/majiang/majiang/majiang.h
        reference/�齫AIԴ����/majiang/majiang/majiangDoc.cpp
        reference/�齫AIԴ����/majiang/majiang/majiangDoc.h
        reference/�齫AIԴ����/majiang/majiang/majiangView.cpp
        reference/�齫AIԴ����/majiang/majiang/majiangView.h
        reference/�齫AIԴ����/majiang/majiang/Method.cpp
        reference/�齫AIԴ����/majiang/majiang/Method.h
        reference/�齫AIԴ����/majiang/majiang/Mp3Sound.cpp
        reference/�齫AIԴ����/majiang/majiang/Mp3Sound.h
        reference/�齫AIԴ����/majiang/majiang/Player_A.cpp
        reference/�齫AIԴ����/majiang/majiang/Player_A.h
        reference/�齫AIԴ����/majiang/majiang/Resource.h
        reference/�齫AIԴ����/majiang/majiang/stdafx.cpp
        reference/�齫AIԴ����/majiang/majiang/stdafx.h
        jsoncpp.cpp
        mahjong.h
        main.cpp)
