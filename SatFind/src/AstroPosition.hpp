/**
 * @file AstroPosition.hpp
 * @author fugu133
 * @brief
 * @version 0.1
 * @date 2023-12-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <iostream>

#include "AngleHelper.hpp"
#include "Coordinate.hpp"
#include "DateTime.hpp"
#include "Eigen/Geometry"
#include "Essential.hpp"
#include "GlobalConstant.hpp"
#include "Polynomial.hpp"

SATFIND_NAMESPACE_BEGIN

struct AstroObjectPositionBase {
  public:
	AstroObjectPositionBase() { initialize(DateTime::now()); }
	AstroObjectPositionBase(const DateTime& dt) { initialize(dt); }
	void update(const DateTime& dt) { initialize(dt); }
	const Eci& eci() const { return m_eci_position; }
	const EclipticSpherical& ecliptic() const { return m_ecliptic_position; }

  private:
	virtual void initialize(const DateTime& dt) { (void)dt; }

  protected:
	Eci m_eci_position;
	EclipticSpherical m_ecliptic_position;
};

/**
 * @brief 太陽の位置を計算するクラス
 * @ref Meeus, Jean, Astronomical Algorithms (2nd Ed.). Richmond: Willmann-Bell, Inc., 2009, Ch. 25.
 *
 */
class SunPosition : public AstroObjectPositionBase {

  public:
	SunPosition() { initialize(DateTime::now()); }

	SunPosition(const DateTime& dt) { initialize(dt); }

	Angle obliquity() const { return obliquity(m_ecliptic_position.epoch()); }

	static Angle obliquity(const DateTime& dt) {
		const double T = (dt.j2000() + dt.deltaT().totalDays()) / constant::jd_century; // Julian centuries since J2000
		const double Omega = AngleHelper::degreeToWrapRadian(125.04 - 1934.136 * T);	// Longitude of ascending node
		return Radian{AngleHelper::degreeToWrapRadian(23 + (26 + Polynomial::deg3(T, 21.448, 46.8150, 0.00059, -0.001813) / 60) / 60 +
													  0.00256 * std::cos(Omega))}; // Obliquity of the ecliptic
	}

  private:
	void initialize(const DateTime& dt) override {
		const double T = (dt.j2000() + dt.deltaT().totalDays()) / constant::jd_century; // Julian centuries since J2000
		const double L0 = AngleHelper::degreeToWrapRadian(Polynomial::deg2(T, 280.46646, 36000.76983, 0.0003032)); // Mean longitude
		const double M = AngleHelper::degreeToWrapRadian(Polynomial::deg2(T, 357.52911, 35999.05029, -0.0001537)); // Mean anomaly
		const double e = Polynomial::deg2(T, 0.016708634, -0.000042037, -0.0000001267);							   // Eccentricity
		const double C =
		  AngleHelper::degreeToWrapRadian((Polynomial::deg2(T, 1.914602, 0.004817, 0.000014) * std::sin(M) +
										   (0.019993 - T * 0.000101) * std::sin(2 * M) + 0.000289 * std::sin(3 * M))); // Equation of center
		const double true_lon = AngleHelper::wrapRadian(L0 + C);													   // True longitude
		const double nu = AngleHelper::wrapRadian(M + C);															   // True anomaly
		const double R = constant::au * (1.000001018 * (1 - e * e) / (1 + e * std::cos(nu))); // Distance correction
		const double Omega = AngleHelper::degreeToWrapRadian(125.04 - 1934.136 * T);		  // Longitude of ascending node
		const double lon =
		  AngleHelper::wrapRadian(true_lon - AngleHelper::degreeToRadian(0.00569 - 0.00478 * std::sin(Omega))); // Apparent longitude

		m_ecliptic_position = EclipticSpherical{dt, Radian{lon}, Angle::zero(), R};
		m_eci_position = m_ecliptic_position.toEci();
	}
};

/**
 * @brief 月の位置を計算するクラス
 * @ref Montenbruck and Gill, Satellite Orbits. Berlin: Springer, 2005, Ch. 3.3.2.
 */
class MoonPosition : public AstroObjectPositionBase {
  public:
	MoonPosition() { initialize(DateTime::now()); }
	MoonPosition(const DateTime& dt) { initialize(dt); }

  private:
	void initialize(const DateTime& dt) override {
		const double T = (dt.j2000() + dt.deltaT().totalDays()) / constant::jd_century;					 // Julian centuries since J2000
		const double L0 = AngleHelper::degreeToWrapRadian(Polynomial::deg1(T, 218.31617, 481267.88088)); // Mean longitude
		const double l = AngleHelper::degreeToWrapRadian(Polynomial::deg1(T, 134.96292, 477198.86753));	 // Mean anomaly
		const double lp = AngleHelper::degreeToWrapRadian(Polynomial::deg1(T, 357.52543, 35999.04944));	 // Mean elongation
		const double F = AngleHelper::degreeToWrapRadian(Polynomial::deg1(T, 93.27283, 483202.01873));	 // Argument of latitude
		const double D = AngleHelper::degreeToWrapRadian(Polynomial::deg1(T, 297.85027, 445267.11135));	 // Mean distance
		const double dL = AngleHelper::degreeToWrapRadian((22640 * std::sin(l) + 769 * std::sin(2 * l) - 4586 * std::sin(l - 2 * D) +
														   2370 * std::sin(2 * D) - 668 * std::sin(lp) - 412 * std::sin(2 * F) -
														   212 * std::sin(2 * l - 2 * D) - 206 * std::sin(l + lp - 2 * D) +
														   192 * std::sin(l + 2 * D) - 165 * std::sin(lp - 2 * D) + 148 * std::sin(l - lp) -
														   125 * std::sin(D) - 110 * std::sin(l + lp) - 55 * std::sin(2 * F - 2 * D)) /
														  3600); // Longitude correction
		const double lon = AngleHelper::wrapRadian(L0 + dL);	 // Corrected longitude
		const double beta = AngleHelper::degreeToWrapRadian(
		  (18520 * std::sin(F + dL + AngleHelper::degreeToWrapRadian((412 * std::sin(2 * F) + 541 * std::sin(lp)) / 3600)) -
		   526 * std::sin(F - 2 * D) + 44 * std::sin(l + F - 2 * D) - 31 * std::sin(-l + F - 2 * D) - 25 * std::sin(-2 * l + F) -
		   23 * std::sin(lp + F - 2 * D) + 21 * std::sin(-l + F) + 11 * std::sin(-lp + F - 2 * D)) /
		  3600); // Latitude correction

		const double r = 1000 * (385000 - 20905 * std::cos(l) - 3699 * std::cos(2 * D - l) - 2956 * std::cos(2 * D) -
								 570 * std::cos(2 * l) + 246 * std::cos(2 * l - 2 * D) - 205 * std::cos(lp - 2 * D) -
								 171 * std::cos(l + 2 * D) - 152 * std::cos(l + lp - 2 * D)); // Distance correction

		m_ecliptic_position = EclipticSpherical{dt, Radian{lon}, Radian{beta}, r};
		m_eci_position = m_ecliptic_position.toEci();
	}
};

SATFIND_NAMESPACE_END