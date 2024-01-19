/**
 * @file GroundObserver.hpp
 * @author fugu133
 * @brief 地上系観測者を表すクラス
 * @version 0.1
 * @date 2023-12-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "Coordinate.hpp"
#include "Essential.hpp"

SATFIND_NAMESPACE_BEGIN

class GroundObserver {
  public:
	/**
	 * @brief Construct a new Ground Observer object
	 *
	 * @param latitude 観測者位置の緯度
	 * @param longitude 観測者位置の経度
	 * @param altitude 観測者位置の高度 [m]
	 */
	GroundObserver(const Angle& longitude, const Angle& latitude, double altitude) : m_position{longitude, latitude, altitude} {}

	/**
	 * @brief Construct a new Ground Observer object
	 *
	 * @param position WGS84での観測者の位置
	 */
	GroundObserver(const Wgs84Position& position) : m_position{position} {}

	/**
	 * @brief Construct a new Ground Observer object
	 *
	 * @param position WGS84での観測者の位置
	 */
	GroundObserver(const Wgs84& wgs84) : GroundObserver(wgs84.elements()) {}

	Topocentric lookUpPosition(const Eci& s_position) const {
		// 地方恒星時
		auto lst = s_position.epoch().localSiderealTime(m_position.longitude);

		// 観測者から衛星への位置ベクトル
		auto g_position = Wgs84{s_position.epoch(), m_position}.toEci();
		Eigen::Vector3d r_eci = s_position.elements() - g_position.elements();

		// ESU座標系に変換
		decltype(r_eci) r_esu;
		const double cos_lat = m_position.latitude.cos();
		const double sin_lat = m_position.latitude.sin();
		const double cos_lst = lst.cos();
		const double sin_lst = lst.sin();

		r_esu.x() = sin_lat * cos_lst * r_eci.x() + sin_lat * sin_lst * r_eci.y() - cos_lat * r_eci.z(); // S
		r_esu.y() = -sin_lst * r_eci.x() + cos_lst * r_eci.y();											 // E
		r_esu.z() = cos_lat * cos_lst * r_eci.x() + cos_lat * sin_lst * r_eci.y() + sin_lat * r_eci.z(); // U

		TopocentricPosition aer;

		aer.range = r_esu.norm();
		aer.azimuth = Radian(std::atan(-r_esu.y() / r_esu.x()));
		if (r_esu.z() > 0.0)
			aer.azimuth += Radian(constant::pi);
		else if (r_esu.z() < 0.0)
			aer.azimuth += Radian(constant::pi2);
		aer.elevation = Radian(std::asin(r_esu.z() / aer.range));

		// 位置ベクトルを極座標系に変換
		return Topocentric{s_position.epoch(), aer};
	};

  private:
	Wgs84Position m_position;
};

SATFIND_NAMESPACE_END
