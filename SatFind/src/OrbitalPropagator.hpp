/**
 * @file OrbitalPropagator.hpp
 * @author fugu133
 * @brief
 * @version 0.1
 * @date 2023-12-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <iostream>

#include "AngleHelper.hpp"
#include "DateTime.hpp"
#include "Essential.hpp"
#include "OrbitalElements.hpp"
#include "Polynomial.hpp"
#include "Tle.hpp"

SATFIND_NAMESPACE_BEGIN

class OrbitalPropagator {
  public:
	OrbitalPropagator(const std::string& line1, const std::string& line2) : m_elements(Tle{line1, line2}) { initialize(); }

	OrbitalPropagator(const Tle& tle) : m_elements(tle) { initialize(); }

	OrbitalPropagator(const OrbitalElements& e) : m_elements(e) { initialize(); }

	OrbitalPropagator(std::istream& stream) : m_elements(Tle{stream}) { initialize(); }

	auto trackFlightObject(const TimeSpan& time_span) -> CartesianOrbitalElements {
		if (m_is_using_deep_space) {
			return propagateSdp4(time_span.totalMinutes());
		} else {
			return propagateSgp4(time_span.totalMinutes());
		}
	}

	auto trackFlightObject(const DateTime& time) -> CartesianOrbitalElements { return trackFlightObject(time - m_elements.epoch); }

  private:
	/**
	 * @brief 共通定数
	 */
	struct CommonConstants {
		double cosio;
		double sinio;
		double eta;
		double t2cof;
		double x1mth2;
		double x3thm1;
		double x7thm1;
		double aycof;
		double xlcof;
		double xnodcf;
		double c1;
		double c4;
		double omgdot; // omegaの経年変化率 (rad/s)
		double xnodot; // xnodeの経年変化率 (rad/s)
		double xmdot;  // xmoの経年変化率   (rad/s)
	};

	/**
	 * @brief 近宇宙定数
	 */
	struct NearSpaceConstants {
		double c5;
		double omgcof;
		double xmcof;
		double delmo;
		double sinmo;
		double d2;
		double d3;
		double d4;
		double t3cof;
		double t4cof;
		double t5cof;
	};

	/**
	 * @brief 深宇宙定数
	 */
	struct DeepSpaceConstants {
		double gsto;
		double zmol;
		double zmos;

		/*
		 * エポックでの月/太陽定数
		 */
		double sse;
		double ssi;
		double ssl;
		double ssg;
		double ssh;

		/*
		 * 月/太陽定数
		 */
		double se2;
		double si2;
		double sl2;
		double sgh2;
		double sh2;
		double se3;
		double si3;
		double sl3;
		double sgh3;
		double sh3;
		double sl4;
		double sgh4;
		double ee2;
		double e3;
		double xi2;
		double xi3;
		double xl2;
		double xl3;
		double xl4;
		double xgh2;
		double xgh3;
		double xgh4;
		double xh2;
		double xh3;

		double d2201;
		double d2211;
		double d3210;
		double d3222;
		double d4410;
		double d4422;
		double d5220;
		double d5232;
		double d5421;
		double d5433;
		double del1;
		double del2;
		double del3;

		double xfact;
		double xlamo;

		enum class OrbitShape { None, Resonance, Synchonous } shape;
	};

	/**
	 * @brief 積分器定数
	 */
	struct IntegratorParams {

		/*
		 * integrator values
		 */
		double xli;
		double xni;
		double atime;
	};

	OrbitalElements m_elements;				   // 軌道要素
	CommonConstants m_common_constants;		   // 共通定数
	NearSpaceConstants m_near_space_constants; // 近宇宙定数
	DeepSpaceConstants m_deep_space_constants; // 深宇宙定数
	IntegratorParams m_integrator_params;	   // 積分器定数
	bool m_is_using_deep_space;				   // 深宇宙モデルを使用するかどうか
	bool m_is_using_simple_model;			   // 簡易モデルを使用するかどうか

	void clear() {
		m_common_constants = {};
		m_near_space_constants = {};
		m_deep_space_constants = {};
		m_integrator_params = {};
		m_is_using_deep_space = false;
		m_is_using_simple_model = false;
	}

	/**
	 * @brief Set the Constant Parameters object
	 *
	 * @param xinc
	 * @param sinio
	 * @param cosio
	 * @param x3thm1
	 * @param x1mth2
	 * @param x7thm1
	 * @param xlcof
	 * @param aycof
	 */
	void setConstantParameters(const double xinc, double& sinio, double& cosio, double& x3thm1, double& x1mth2, double& x7thm1,
							   double& xlcof, double& aycof) {
		sinio = std::sin(xinc);
		cosio = std::cos(xinc);

		const double theta2 = cosio * cosio;

		x3thm1 = 3.0 * theta2 - 1.0;
		x1mth2 = 1.0 - theta2;
		x7thm1 = 7.0 * theta2 - 1.0;

		if (std::fabs(cosio + 1.0) > 1.5e-12) {
			xlcof = 0.125 * constant::a3ovk2 * sinio * (3.0 + 5.0 * cosio) / (1.0 + cosio);
		} else {
			xlcof = 0.125 * constant::a3ovk2 * sinio * (3.0 + 5.0 * cosio) / 1.5e-12;
		}

		aycof = 0.25 * constant::a3ovk2 * sinio;
	}

	void initialize() {
		clear();

		if (m_elements.eccentricity < 0.0 || m_elements.eccentricity > 0.999) {
			throw OrbitException("Eccentricity out of range", OrbitException::ParameterOutOfRange);
		}

		if (m_elements.inclination < 0.0 || m_elements.inclination > constant::pi) {
			throw OrbitException("Inclination out of range", OrbitException::ParameterOutOfRange);
		}

		setConstantParameters(m_elements.inclination, m_common_constants.sinio, m_common_constants.cosio, m_common_constants.x3thm1,
							  m_common_constants.x1mth2, m_common_constants.x7thm1, m_common_constants.xlcof, m_common_constants.aycof);

		const double theta2 = m_common_constants.cosio * m_common_constants.cosio;
		const double eosq = m_elements.eccentricity * m_elements.eccentricity;
		const double betao2 = 1.0 - eosq;
		const double betao = sqrt(betao2);

		if (m_elements.period >= 225.0) {
			m_is_using_deep_space = true;
		} else {
			m_is_using_deep_space = false;
			m_is_using_simple_model = false;

			if (m_elements.perigee < 220.0) {
				m_is_using_simple_model = true;
			}
		}

		double s4 = constant::s;
		double qoms24 = constant::qoms2t;
		if (m_elements.perigee < 156.0) {
			s4 = m_elements.perigee - 78.0;
			if (m_elements.perigee < 98.0) {
				s4 = 20.0;
			}
			qoms24 = pow((120.0 - s4) * constant::ae / constant::xkmper, 4.0);
			s4 = s4 / constant::xkmper + constant::ae;
		}

		const double pinvsq = 1.0 / (m_elements.recovered_semi_major_axis * m_elements.recovered_semi_major_axis * betao2 * betao2);
		const double tsi = 1.0 / (m_elements.recovered_semi_major_axis - s4);
		m_common_constants.eta = m_elements.recovered_semi_major_axis * m_elements.eccentricity * tsi;
		const double etasq = m_common_constants.eta * m_common_constants.eta;
		const double eeta = m_elements.eccentricity * m_common_constants.eta;
		const double psisq = fabs(1.0 - etasq);
		const double coef = qoms24 * pow(tsi, 4.0);
		const double coef1 = coef / pow(psisq, 3.5);
		const double c2 = coef1 * m_elements.recovered_mean_motion *
						  (m_elements.recovered_semi_major_axis * (1.0 + 1.5 * etasq + eeta * (4.0 + etasq)) +
						   0.75 * constant::ck2 * tsi / psisq * m_common_constants.x3thm1 * (8.0 + 3.0 * etasq * (8.0 + etasq)));
		m_common_constants.c1 = m_elements.b_star * c2;
		m_common_constants.c4 =
		  2.0 * m_elements.recovered_mean_motion * coef1 * m_elements.recovered_semi_major_axis * betao2 *
		  (m_common_constants.eta * (2.0 + 0.5 * etasq) + m_elements.eccentricity * (0.5 + 2.0 * etasq) -
		   2.0 * constant::ck2 * tsi / (m_elements.recovered_semi_major_axis * psisq) *
			 (-3.0 * m_common_constants.x3thm1 * (1.0 - 2.0 * eeta + etasq * (1.5 - 0.5 * eeta)) +
			  0.75 * m_common_constants.x1mth2 * (2.0 * etasq - eeta * (1.0 + etasq)) * cos(2.0 * m_elements.argument_perigee)));
		const double theta4 = theta2 * theta2;
		const double temp1 = 3.0 * constant::ck2 * pinvsq * m_elements.recovered_mean_motion;
		const double temp2 = temp1 * constant::ck2 * pinvsq;
		const double temp3 = 1.25 * constant::ck4 * pinvsq * pinvsq * m_elements.recovered_mean_motion;
		m_common_constants.xmdot = m_elements.recovered_mean_motion + 0.5 * temp1 * betao * m_common_constants.x3thm1 +
								   0.0625 * temp2 * betao * (13.0 - 78.0 * theta2 + 137.0 * theta4);
		const double x1m5th = 1.0 - 5.0 * theta2;
		m_common_constants.omgdot =
		  -0.5 * temp1 * x1m5th + 0.0625 * temp2 * (7.0 - 114.0 * theta2 + 395.0 * theta4) + temp3 * (3.0 - 36.0 * theta2 + 49.0 * theta4);
		const double xhdot1 = -temp1 * m_common_constants.cosio;
		m_common_constants.xnodot =
		  xhdot1 + (0.5 * temp2 * (4.0 - 19.0 * theta2) + 2.0 * temp3 * (3.0 - 7.0 * theta2)) * m_common_constants.cosio;
		m_common_constants.xnodcf = 3.5 * betao2 * xhdot1 * m_common_constants.c1;
		m_common_constants.t2cof = 1.5 * m_common_constants.c1;

		if (m_is_using_deep_space) {
			m_deep_space_constants.gsto = m_elements.epoch.greenwichSiderealTime().radians();

			initializeDeepSpaceConstants(eosq, m_common_constants.sinio, m_common_constants.cosio, betao, theta2, betao2,
										 m_common_constants.xmdot, m_common_constants.omgdot, m_common_constants.xnodot);
		} else {
			double c3 = 0.0;
			if (m_elements.eccentricity > 1.0e-4) {
				c3 = coef * tsi * constant::a3ovk2 * m_elements.recovered_mean_motion * constant::ae * m_common_constants.sinio /
					 m_elements.eccentricity;
			}

			m_near_space_constants.c5 =
			  2.0 * coef1 * m_elements.recovered_semi_major_axis * betao2 * (1.0 + 2.75 * (etasq + eeta) + eeta * etasq);
			m_near_space_constants.omgcof = m_elements.b_star * c3 * cos(m_elements.argument_perigee);

			m_near_space_constants.xmcof = 0.0;
			if (m_elements.eccentricity > 1.0e-4) {
				m_near_space_constants.xmcof = -constant::tow_third * coef * m_elements.b_star * constant::ae / eeta;
			}

			m_near_space_constants.delmo = pow(1.0 + m_common_constants.eta * (cos(m_elements.mean_anomaly)), 3.0);
			m_near_space_constants.sinmo = sin(m_elements.mean_anomaly);

			if (!m_is_using_simple_model) {
				const double c1sq = m_common_constants.c1 * m_common_constants.c1;
				m_near_space_constants.d2 = 4.0 * m_elements.recovered_semi_major_axis * tsi * c1sq;
				const double temp = m_near_space_constants.d2 * tsi * m_common_constants.c1 / 3.0;
				m_near_space_constants.d3 = (17.0 * m_elements.recovered_semi_major_axis + s4) * temp;
				m_near_space_constants.d4 = 0.5 * temp * m_elements.recovered_semi_major_axis * tsi *
											(221.0 * m_elements.recovered_semi_major_axis + 31.0 * s4) * m_common_constants.c1;
				m_near_space_constants.t3cof = m_near_space_constants.d2 + 2.0 * c1sq;
				m_near_space_constants.t4cof =
				  0.25 * (3.0 * m_near_space_constants.d3 + m_common_constants.c1 * (12.0 * m_near_space_constants.d2 + 10.0 * c1sq));
				m_near_space_constants.t5cof =
				  0.2 *
				  (3.0 * m_near_space_constants.d4 + 12.0 * m_common_constants.c1 * m_near_space_constants.d3 +
				   6.0 * m_near_space_constants.d2 * m_near_space_constants.d2 + 15.0 * c1sq * (2.0 * m_near_space_constants.d2 + c1sq));
			}
		}
	}

	void initializeDeepSpaceConstants(const double eosq, const double sinio, const double cosio, const double betao, const double theta2,
									  const double betao2, const double xmdot, const double omgdot, const double xnodot) {
		double se = 0.0;
		double si = 0.0;
		double sl = 0.0;
		double sgh = 0.0;
		double shdq = 0.0;

		double bfact = 0.0;

		constexpr double ZNS = 1.19459E-5;
		constexpr double C1SS = 2.9864797E-6;
		constexpr double ZES = 0.01675;
		constexpr double ZNL = 1.5835218E-4;
		constexpr double C1L = 4.7968065E-7;
		constexpr double ZEL = 0.05490;
		constexpr double ZCOSIS = 0.91744867;
		constexpr double ZSINI = 0.39785416;
		constexpr double ZSINGS = -0.98088458;
		constexpr double ZCOSGS = 0.1945905;
		constexpr double Q22 = 1.7891679E-6;
		constexpr double Q31 = 2.1460748E-6;
		constexpr double Q33 = 2.2123015E-7;
		constexpr double ROOT22 = 1.7891679E-6;
		constexpr double ROOT32 = 3.7393792E-7;
		constexpr double ROOT44 = 7.3636953E-9;
		constexpr double ROOT52 = 1.1428639E-7;
		constexpr double ROOT54 = 2.1765803E-9;

		const double aqnv = 1.0 / m_elements.recovered_semi_major_axis;
		const double xpidot = omgdot + xnodot;
		const double sinq = std::sin(m_elements.ascending_node);
		const double cosq = std::cos(m_elements.ascending_node);
		const double sing = std::sin(m_elements.argument_perigee);
		const double cosg = std::cos(m_elements.argument_perigee);

		const double jday = m_elements.epoch.j2000();

		const double xnodce = AngleHelper::wrapRadian(4.5236020 - 9.2422029e-4 * jday);
		const double stem = std::sin(xnodce);
		const double ctem = std::cos(xnodce);
		const double zcosil = 0.91375164 - 0.03568096 * ctem;
		const double zsinil = std::sqrt(1.0 - zcosil * zcosil);
		const double zsinhl = 0.089683511 * stem / zsinil;
		const double zcoshl = std::sqrt(1.0 - zsinhl * zsinhl);
		const double c = 4.7199672 + 0.22997150 * jday;
		const double gam = 5.8351514 + 0.0019443680 * jday;
		m_deep_space_constants.zmol = AngleHelper::wrapRadian(c - gam);
		double zx = 0.39785416 * stem / zsinil;
		double zy = zcoshl * ctem + 0.91744867 * zsinhl * stem;
		zx = std::atan2(zx, zy);
		zx = gam + zx - xnodce;

		const double zcosgl = std::cos(zx);
		const double zsingl = std::sin(zx);
		m_deep_space_constants.zmos = AngleHelper::wrapRadian(6.2565837 + 0.017201977 * jday);

		double zcosg = ZCOSGS;
		double zsing = ZSINGS;
		double zcosi = ZCOSIS;
		double zsini = ZSINI;
		double zcosh = cosq;
		double zsinh = sinq;
		double cc = C1SS;
		double zn = ZNS;
		double ze = ZES;
		const double xnoi = 1.0 / m_elements.recovered_mean_motion;

		for (int cnt = 0; cnt < 2; cnt++) {
			const double a1 = zcosg * zcosh + zsing * zcosi * zsinh;
			const double a3 = -zsing * zcosh + zcosg * zcosi * zsinh;
			const double a7 = -zcosg * zsinh + zsing * zcosi * zcosh;
			const double a8 = zsing * zsini;
			const double a9 = zsing * zsinh + zcosg * zcosi * zcosh;
			const double a10 = zcosg * zsini;
			const double a2 = cosio * a7 + sinio * a8;
			const double a4 = cosio * a9 + sinio * a10;
			const double a5 = -sinio * a7 + cosio * a8;
			const double a6 = -sinio * a9 + cosio * a10;
			const double x1 = a1 * cosg + a2 * sing;
			const double x2 = a3 * cosg + a4 * sing;
			const double x3 = -a1 * sing + a2 * cosg;
			const double x4 = -a3 * sing + a4 * cosg;
			const double x5 = a5 * sing;
			const double x6 = a6 * sing;
			const double x7 = a5 * cosg;
			const double x8 = a6 * cosg;
			const double z31 = 12.0 * x1 * x1 - 3. * x3 * x3;
			const double z32 = 24.0 * x1 * x2 - 6. * x3 * x4;
			const double z33 = 12.0 * x2 * x2 - 3. * x4 * x4;
			double z1 = 3.0 * (a1 * a1 + a2 * a2) + z31 * eosq;
			double z2 = 6.0 * (a1 * a3 + a2 * a4) + z32 * eosq;
			double z3 = 3.0 * (a3 * a3 + a4 * a4) + z33 * eosq;

			const double z11 = -6.0 * a1 * a5 + eosq * (-24. * x1 * x7 - 6. * x3 * x5);
			const double z12 = -6.0 * (a1 * a6 + a3 * a5) + eosq * (-24. * (x2 * x7 + x1 * x8) - 6. * (x3 * x6 + x4 * x5));
			const double z13 = -6.0 * a3 * a6 + eosq * (-24. * x2 * x8 - 6. * x4 * x6);
			const double z21 = 6.0 * a2 * a5 + eosq * (24. * x1 * x5 - 6. * x3 * x7);
			const double z22 = 6.0 * (a4 * a5 + a2 * a6) + eosq * (24. * (x2 * x5 + x1 * x6) - 6. * (x4 * x7 + x3 * x8));
			const double z23 = 6.0 * a4 * a6 + eosq * (24. * x2 * x6 - 6. * x4 * x8);

			z1 = z1 + z1 + betao2 * z31;
			z2 = z2 + z2 + betao2 * z32;
			z3 = z3 + z3 + betao2 * z33;

			const double s3 = cc * xnoi;
			const double s2 = -0.5 * s3 / betao;
			const double s4 = s3 * betao;
			const double s1 = -15.0 * m_elements.eccentricity * s4;
			const double s5 = x1 * x3 + x2 * x4;
			const double s6 = x2 * x3 + x1 * x4;
			const double s7 = x2 * x4 - x1 * x3;

			se = s1 * zn * s5;
			si = s2 * zn * (z11 + z13);
			sl = -zn * s3 * (z1 + z3 - 14.0 - 6.0 * eosq);
			sgh = s4 * zn * (z31 + z33 - 6.0);

			if (m_elements.inclination < 5.2359877e-2 || m_elements.inclination > constant::pi - 5.2359877e-2) {
				shdq = 0.0;
			} else {
				shdq = (-zn * s2 * (z21 + z23)) / sinio;
			}

			m_deep_space_constants.ee2 = 2.0 * s1 * s6;
			m_deep_space_constants.e3 = 2.0 * s1 * s7;
			m_deep_space_constants.xi2 = 2.0 * s2 * z12;
			m_deep_space_constants.xi3 = 2.0 * s2 * (z13 - z11);
			m_deep_space_constants.xl2 = -2.0 * s3 * z2;
			m_deep_space_constants.xl3 = -2.0 * s3 * (z3 - z1);
			m_deep_space_constants.xl4 = -2.0 * s3 * (-21.0 - 9.0 * eosq) * ze;
			m_deep_space_constants.xgh2 = 2.0 * s4 * z32;
			m_deep_space_constants.xgh3 = 2.0 * s4 * (z33 - z31);
			m_deep_space_constants.xgh4 = -18.0 * s4 * ze;
			m_deep_space_constants.xh2 = -2.0 * s2 * z22;
			m_deep_space_constants.xh3 = -2.0 * s2 * (z23 - z21);

			if (cnt == 1) {
				break;
			}

			m_deep_space_constants.sse = se;
			m_deep_space_constants.ssi = si;
			m_deep_space_constants.ssl = sl;
			m_deep_space_constants.ssh = shdq;
			m_deep_space_constants.ssg = sgh - cosio * m_deep_space_constants.ssh;
			m_deep_space_constants.se2 = m_deep_space_constants.ee2;
			m_deep_space_constants.si2 = m_deep_space_constants.xi2;
			m_deep_space_constants.sl2 = m_deep_space_constants.xl2;
			m_deep_space_constants.sgh2 = m_deep_space_constants.xgh2;
			m_deep_space_constants.sh2 = m_deep_space_constants.xh2;
			m_deep_space_constants.se3 = m_deep_space_constants.e3;
			m_deep_space_constants.si3 = m_deep_space_constants.xi3;
			m_deep_space_constants.sl3 = m_deep_space_constants.xl3;
			m_deep_space_constants.sgh3 = m_deep_space_constants.xgh3;
			m_deep_space_constants.sh3 = m_deep_space_constants.xh3;
			m_deep_space_constants.sl4 = m_deep_space_constants.xl4;
			m_deep_space_constants.sgh4 = m_deep_space_constants.xgh4;
			zcosg = zcosgl;
			zsing = zsingl;
			zcosi = zcosil;
			zsini = zsinil;
			zcosh = zcoshl * cosq + zsinhl * sinq;
			zsinh = sinq * zcoshl - cosq * zsinhl;
			zn = ZNL;
			cc = C1L;
			ze = ZEL;
		}

		m_deep_space_constants.sse += se;
		m_deep_space_constants.ssi += si;
		m_deep_space_constants.ssl += sl;
		m_deep_space_constants.ssg += sgh - cosio * shdq;
		m_deep_space_constants.ssh += shdq;

		m_deep_space_constants.shape = DeepSpaceConstants::OrbitShape::None;

		if (m_elements.recovered_mean_motion < 0.0052359877 && m_elements.recovered_mean_motion > 0.0034906585) {
			m_deep_space_constants.shape = DeepSpaceConstants::OrbitShape::Synchonous;

			const double g200 = 1.0 + eosq * (-2.5 + 0.8125 * eosq);
			const double g310 = 1.0 + 2.0 * eosq;
			const double g300 = 1.0 + eosq * (-6.0 + 6.60937 * eosq);
			const double f220 = 0.75 * (1.0 + cosio) * (1.0 + cosio);
			const double f311 = 0.9375 * sinio * sinio * (1.0 + 3.0 * cosio) - 0.75 * (1.0 + cosio);
			double f330 = 1.0 + cosio;
			f330 = 1.875 * f330 * f330 * f330;
			m_deep_space_constants.del1 = 3.0 * m_elements.recovered_mean_motion * m_elements.recovered_mean_motion * aqnv * aqnv;
			m_deep_space_constants.del2 = 2.0 * m_deep_space_constants.del1 * f220 * g200 * Q22;
			m_deep_space_constants.del3 = 3.0 * m_deep_space_constants.del1 * f330 * g300 * Q33 * aqnv;
			m_deep_space_constants.del1 = m_deep_space_constants.del1 * f311 * g310 * Q31 * aqnv;

			m_deep_space_constants.xlamo = AngleHelper::wrapRadian(m_elements.mean_anomaly + m_elements.ascending_node +
																   m_elements.argument_perigee - m_deep_space_constants.gsto);
			bfact = xmdot + xpidot - constant::thdt + m_deep_space_constants.ssl + m_deep_space_constants.ssg + m_deep_space_constants.ssh;
		} else if (m_elements.recovered_mean_motion < 8.26e-3 || m_elements.recovered_mean_motion > 9.24e-3 ||
				   m_elements.eccentricity < 0.5) {
			// do nothing
		} else {
			/*
			 * geopotential resonance initialisation for 12 hour orbits
			 */
			m_deep_space_constants.shape = DeepSpaceConstants::OrbitShape::Resonance;

			double g211;
			double g310;
			double g322;
			double g410;
			double g422;
			double g520;

			double g201 = -0.306 - (m_elements.eccentricity - 0.64) * 0.440;

			if (m_elements.eccentricity <= 0.65) {
				g211 = Polynomial::deg3(m_elements.eccentricity, 3.616, -13.247, 16.290, 0.0);
				g310 = Polynomial::deg3(m_elements.eccentricity, -19.302, 117.390, -228.419, 156.591);
				g322 = Polynomial::deg3(m_elements.eccentricity, -18.9068, 109.7927, -214.6334, 146.5816);
				g410 = Polynomial::deg3(m_elements.eccentricity, -41.122, 242.694, -471.094, 313.953);
				g422 = Polynomial::deg3(m_elements.eccentricity, -146.407, 841.880, -1629.014, 1083.435);
				g520 = Polynomial::deg3(m_elements.eccentricity, -532.114, 3017.977, -5740.032, 3708.276);
			} else {
				g211 = Polynomial::deg3(m_elements.eccentricity, -72.099, 331.819, -508.738, 266.724);
				g310 = Polynomial::deg3(m_elements.eccentricity, -346.844, 1582.851, -2415.925, 1246.113);
				g322 = Polynomial::deg3(m_elements.eccentricity, -342.585, 1554.908, -2366.899, 1215.972);
				g410 = Polynomial::deg3(m_elements.eccentricity, -1052.797, 4758.686, -7193.992, 3651.957);
				g422 = Polynomial::deg3(m_elements.eccentricity, -3581.69, 16178.11, -24462.77, 12422.52);

				if (m_elements.eccentricity <= 0.715) {
					g520 = Polynomial::deg3(m_elements.eccentricity, 1464.74, -4664.75, 3763.64, 0.0);
				} else {
					g520 = Polynomial::deg3(m_elements.eccentricity, -5149.66, 29936.92, -54087.36, 31324.56);
				}
			}

			double g533;
			double g521;
			double g532;

			if (m_elements.eccentricity < 0.7) {
				g533 = Polynomial::deg3(m_elements.eccentricity, -919.2277, 4988.61, -9064.77, 5542.21);
				g521 = Polynomial::deg3(m_elements.eccentricity, -822.71072, 4568.6173, -8491.4146, 5337.524);
				g532 = Polynomial::deg3(m_elements.eccentricity, -853.666, 4690.25, -8624.77, 5341.4);
			} else {
				g533 = Polynomial::deg3(m_elements.eccentricity, -37995.78, 161616.52, -229838.2, 109377.94);
				g521 = Polynomial::deg3(m_elements.eccentricity, -51752.104, 218913.95, -309468.16, 146349.42);
				g532 = Polynomial::deg3(m_elements.eccentricity, -40023.88, 170470.89, -242699.48, 115605.82);
			}

			const double sini2 = sinio * sinio;
			const double f220 = 0.75 * (1.0 + 2.0 * cosio + theta2);
			const double f221 = 1.5 * sini2;
			const double f321 = 1.875 * sinio * (1.0 - 2.0 * cosio - 3.0 * theta2);
			const double f322 = -1.875 * sinio * (1.0 + 2.0 * cosio - 3.0 * theta2);
			const double f441 = 35.0 * sini2 * f220;
			const double f442 = 39.3750 * sini2 * sini2;
			const double f522 =
			  9.84375 * sinio * (sini2 * (1.0 - 2.0 * cosio - 5.0 * theta2) + 0.33333333 * (-2.0 + 4.0 * cosio + 6.0 * theta2));
			const double f523 =
			  sinio * (4.92187512 * sini2 * (-2.0 - 4.0 * cosio + 10.0 * theta2) + 6.56250012 * (1.0 + 2.0 * cosio - 3.0 * theta2));
			const double f542 = 29.53125 * sinio * (2.0 - 8.0 * cosio + theta2 * (-12.0 + 8.0 * cosio + 10.0 * theta2));
			const double f543 = 29.53125 * sinio * (-2.0 - 8.0 * cosio + theta2 * (12.0 + 8.0 * cosio - 10.0 * theta2));

			const double xno2 = m_elements.recovered_mean_motion * m_elements.recovered_mean_motion;
			const double ainv2 = aqnv * aqnv;

			double temp1 = 3.0 * xno2 * ainv2;
			double temp = temp1 * ROOT22;
			m_deep_space_constants.d2201 = temp * f220 * g201;
			m_deep_space_constants.d2211 = temp * f221 * g211;

			temp1 *= aqnv;
			temp = temp1 * ROOT32;
			m_deep_space_constants.d3210 = temp * f321 * g310;
			m_deep_space_constants.d3222 = temp * f322 * g322;

			temp1 *= aqnv;
			temp = 2.0 * temp1 * ROOT44;
			m_deep_space_constants.d4410 = temp * f441 * g410;
			m_deep_space_constants.d4422 = temp * f442 * g422;

			temp1 *= aqnv;
			temp = temp1 * ROOT52;
			m_deep_space_constants.d5220 = temp * f522 * g520;
			m_deep_space_constants.d5232 = temp * f523 * g532;

			temp = 2.0 * temp1 * ROOT54;
			m_deep_space_constants.d5421 = temp * f542 * g521;
			m_deep_space_constants.d5433 = temp * f543 * g533;

			m_deep_space_constants.xlamo =
			  AngleHelper::wrapRadian(m_elements.mean_anomaly + m_elements.ascending_node + m_elements.ascending_node -
									  m_deep_space_constants.gsto - m_deep_space_constants.gsto);
			bfact = xmdot + xnodot + xnodot - constant::thdt - constant::thdt + m_deep_space_constants.ssl + m_deep_space_constants.ssh +
					m_deep_space_constants.ssh;
		}

		if (m_deep_space_constants.shape != DeepSpaceConstants::OrbitShape::None) {
			m_deep_space_constants.xfact = bfact - m_elements.recovered_mean_motion;
			m_integrator_params.atime = 0.0;
			m_integrator_params.xni = m_elements.recovered_mean_motion;
			m_integrator_params.xli = m_deep_space_constants.xlamo;
		}
	}
	auto calclateCartesianOrbitalElements(const DateTime& dt, const double e, const double a, const double omega, const double xl,
										  const double xnode, const double xinc, const double xlcof, const double aycof,
										  const double x3thm1, const double x1mth2, const double x7thm1, const double cosio,
										  const double sinio) -> CartesianOrbitalElements {
		const double beta2 = 1.0 - e * e;
		const double xn = constant::xke / std::pow(a, 1.5);

		const double axn = e * std::cos(omega);
		const double temp11 = 1.0 / (a * beta2);
		const double xll = temp11 * xlcof * axn;
		const double aynl = temp11 * aycof;
		const double xlt = xl + xll;
		const double ayn = e * sin(omega) + aynl;
		const double elsq = axn * axn + ayn * ayn;

		if (elsq >= 1.0) {
			throw OrbitException("Error: (elsq >= 1.0)", OrbitException::LongPeriodPredictionError);
		}

		/*
		 * ニュートン・ラプソン法でケプラー方程式を解く
		 * 収束しない場合は, 最大10回まで反復
		 */
		const double capu = std::fmod(xlt - xnode, constant::pi2);
		double epw = capu;

		double sinepw = 0.0;
		double cosepw = 0.0;
		double ecose = 0.0;
		double esine = 0.0;

		const double max_newton_naphson = 1.25 * std::fabs(std::sqrt(elsq));

		bool kepler_running = true;

		for (int i = 0; i < 10 && kepler_running; i++) {
			sinepw = std::sin(epw);
			cosepw = std::cos(epw);
			ecose = axn * cosepw + ayn * sinepw;
			esine = axn * sinepw - ayn * cosepw;

			double f = capu - epw + esine;

			if (std::fabs(f) < 1.0e-12) {
				kepler_running = false;
			} else {
				const double fdot = 1.0 - ecose;
				double delta_epw = f / fdot;

				if (i == 0) {
					if (delta_epw > max_newton_naphson) {
						delta_epw = max_newton_naphson;
					} else if (delta_epw < -max_newton_naphson) {
						delta_epw = -max_newton_naphson;
					}
				} else {
					delta_epw = f / (fdot + 0.5 * esine * delta_epw);
				}

				epw += delta_epw;
			}
		}

		const double temp21 = 1.0 - elsq;
		const double pl = a * temp21;

		if (pl < 0.0) {
			throw OrbitException("Error: (pl < 0.0)", OrbitException::ShortPeriodPredictionError);
		}

		const double r = a * (1.0 - ecose);
		const double temp31 = 1.0 / r;
		const double rdot = constant::xke * std::sqrt(a) * esine * temp31;
		const double rfdot = constant::xke * std::sqrt(pl) * temp31;
		const double temp32 = a * temp31;
		const double betal = sqrt(temp21);
		const double temp33 = 1.0 / (1.0 + betal);
		const double cosu = temp32 * (cosepw - axn + ayn * esine * temp33);
		const double sinu = temp32 * (sinepw - ayn - axn * esine * temp33);
		const double u = atan2(sinu, cosu);
		const double sin2u = 2.0 * sinu * cosu;
		const double cos2u = 2.0 * cosu * cosu - 1.0;

		const double temp41 = 1.0 / pl;
		const double temp42 = constant::ck2 * temp41;
		const double temp43 = temp42 * temp41;

		const double rk = r * (1.0 - 1.5 * temp43 * betal * x3thm1) + 0.5 * temp42 * x1mth2 * cos2u;
		const double uk = u - 0.25 * temp43 * x7thm1 * sin2u;
		const double xnodek = xnode + 1.5 * temp43 * cosio * sin2u;
		const double xinck = xinc + 1.5 * temp43 * cosio * sinio * cos2u;
		const double rdotk = rdot - xn * temp42 * x1mth2 * sin2u;
		const double rfdotk = rfdot + xn * temp42 * (x1mth2 * cos2u + 1.5 * x3thm1);

		/*
		 * 方向ベクトルの計算
		 */
		const double sinuk = std::sin(uk);
		const double cosuk = std::cos(uk);
		const double sinik = std::sin(xinck);
		const double cosik = std::cos(xinck);
		const double sinnok = std::sin(xnodek);
		const double cosnok = std::cos(xnodek);
		const double xmx = -sinnok * cosik;
		const double xmy = cosnok * cosik;
		const double ux = xmx * sinuk + cosnok * cosuk;
		const double uy = xmy * sinuk + sinnok * cosuk;
		const double uz = sinik * sinuk;
		const double vx = xmx * cosuk - cosnok * sinuk;
		const double vy = xmy * cosuk - sinnok * sinuk;
		const double vz = sinik * cosuk;

		/*
		 * 位置/速度ベクトルの計算
		 */
		const double x = rk * ux * constant::xkmper;
		const double y = rk * uy * constant::xkmper;
		const double z = rk * uz * constant::xkmper;
		Eci position(dt, Eigen::Vector3d(x, y, z) * 1e3);
		const double xdot = (rdotk * ux + rfdotk * vx) * constant::xkmper / 60.0;
		const double ydot = (rdotk * uy + rfdotk * vy) * constant::xkmper / 60.0;
		const double zdot = (rdotk * uz + rfdotk * vz) * constant::xkmper / 60.0;
		Eci velocity(dt, Eigen::Vector3d(xdot, ydot, zdot) * 1e3);

		if (rk < 1.0) {
			throw OrbitException("Error: (rk < 1.0)", OrbitException::ObjectDecayed);
		}

		return CartesianOrbitalElements{dt, position, velocity};
	}

	auto deepSpaceSecular(const double tsince, const OrbitalElements& elements, const CommonConstants& c_constants,
						  const DeepSpaceConstants& ds_constants, IntegratorParams& integ_params, double& xll, double& omgasm,
						  double& xnodes, double& em, double& xinc, double& xn) -> void {
		static const double G22 = 5.7686396;
		static const double G32 = 0.95240898;
		static const double G44 = 1.8014998;
		static const double G52 = 1.0508330;
		static const double G54 = 4.4108898;
		static const double FASX2 = 0.13130908;
		static const double FASX4 = 2.8843198;
		static const double FASX6 = 0.37448087;

		static const double STEP = 720.0;
		static const double STEP2 = 259200.0;

		xll += ds_constants.ssl * tsince;
		omgasm += ds_constants.ssg * tsince;
		xnodes += ds_constants.ssh * tsince;
		em += ds_constants.sse * tsince;
		xinc += ds_constants.ssi * tsince;

		if (ds_constants.shape != DeepSpaceConstants::OrbitShape::None) {
			double xndot = 0.0;
			double xnddt = 0.0;
			double xldot = 0.0;

			if (std::fabs(tsince) < STEP || tsince * integ_params.atime <= 0.0 || std::fabs(tsince) < std::fabs(integ_params.atime)) {
				integ_params.atime = 0.0;
				integ_params.xni = elements.recovered_mean_motion;
				integ_params.xli = ds_constants.xlamo;
			}

			bool running = true;
			while (running) {
				if (ds_constants.shape == DeepSpaceConstants::OrbitShape::Synchonous) {
					xndot = ds_constants.del1 * std::sin(integ_params.xli - FASX2) +
							ds_constants.del2 * std::sin(2.0 * (integ_params.xli - FASX4)) +
							ds_constants.del3 * std::sin(3.0 * (integ_params.xli - FASX6));
					xnddt = ds_constants.del1 * std::cos(integ_params.xli - FASX2) +
							2.0 * ds_constants.del2 * std::cos(2.0 * (integ_params.xli - FASX4)) +
							3.0 * ds_constants.del3 * std::cos(3.0 * (integ_params.xli - FASX6));
				} else {
					const double xomi = elements.argument_perigee + c_constants.omgdot * integ_params.atime;
					const double x2omi = xomi + xomi;
					const double x2li = integ_params.xli + integ_params.xli;
					xndot =
					  ds_constants.d2201 * std::sin(x2omi + integ_params.xli - G22) +
					  ds_constants.d2211 * std::sin(integ_params.xli - G22) + ds_constants.d3210 * std::sin(xomi + integ_params.xli - G32) +
					  ds_constants.d3222 * std::sin(-xomi + integ_params.xli - G32) + ds_constants.d4410 * std::sin(x2omi + x2li - G44) +
					  ds_constants.d4422 * std::sin(x2li - G44) + ds_constants.d5220 * std::sin(xomi + integ_params.xli - G52) +
					  ds_constants.d5232 * std::sin(-xomi + integ_params.xli - G52) + ds_constants.d5421 * std::sin(xomi + x2li - G54) +
					  ds_constants.d5433 * std::sin(-xomi + x2li - G54);
					xnddt = ds_constants.d2201 * std::cos(x2omi + integ_params.xli - G22) +
							ds_constants.d2211 * std::cos(integ_params.xli - G22) +
							ds_constants.d3210 * std::cos(xomi + integ_params.xli - G32) +
							ds_constants.d3222 * std::cos(-xomi + integ_params.xli - G32) +
							ds_constants.d5220 * std::cos(xomi + integ_params.xli - G52) +
							ds_constants.d5232 * std::cos(-xomi + integ_params.xli - G52) +
							2.0 * (ds_constants.d4410 * std::cos(x2omi + x2li - G44) + ds_constants.d4422 * std::cos(x2li - G44) +
								   ds_constants.d5421 * std::cos(xomi + x2li - G54) + ds_constants.d5433 * std::cos(-xomi + x2li - G54));
				}
				xldot = integ_params.xni + ds_constants.xfact;
				xnddt *= xldot;

				double ft = tsince - integ_params.atime;
				if (std::fabs(ft) >= STEP) {
					const double delt = (ft >= 0.0 ? STEP : -STEP);
					integ_params.xli = integ_params.xli + xldot * delt + xndot * STEP2;
					integ_params.xni = integ_params.xni + xndot * delt + xnddt * STEP2;
					integ_params.atime += delt;
				} else {
					xn = integ_params.xni + xndot * ft + xnddt * ft * ft * 0.5;
					const double xl_temp = integ_params.xli + xldot * ft + xndot * ft * ft * 0.5;

					const double theta = AngleHelper::wrapRadian(ds_constants.gsto + tsince * constant::thdt);
					if (ds_constants.shape == DeepSpaceConstants::OrbitShape::Synchonous) {
						xll = xl_temp + theta - xnodes - omgasm;
					} else {
						xll = xl_temp + 2.0 * (theta - xnodes);
					}
					running = false;
				}
			}
		}
	}

	auto deepSpacePeriodics(const double tsince, const DeepSpaceConstants& ds_constants, double& em, double& xinc, double& omgasm,
							double& xnodes, double& xll) -> void {
		static const double ZES = 0.01675;
		static const double ZNS = 1.19459E-5;
		static const double ZNL = 1.5835218E-4;
		static const double ZEL = 0.05490;

		double zm = ds_constants.zmos + ZNS * tsince;
		double zf = zm + 2.0 * ZES * std::sin(zm);
		double sinzf = std::sin(zf);
		double f2 = 0.5 * sinzf * sinzf - 0.25;
		double f3 = -0.5 * sinzf * std::cos(zf);

		const double ses = ds_constants.se2 * f2 + ds_constants.se3 * f3;
		const double sis = ds_constants.si2 * f2 + ds_constants.si3 * f3;
		const double sls = ds_constants.sl2 * f2 + ds_constants.sl3 * f3 + ds_constants.sl4 * sinzf;
		const double sghs = ds_constants.sgh2 * f2 + ds_constants.sgh3 * f3 + ds_constants.sgh4 * sinzf;
		const double shs = ds_constants.sh2 * f2 + ds_constants.sh3 * f3;

		zm = ds_constants.zmol + ZNL * tsince;
		zf = zm + 2.0 * ZEL * std::sin(zm);
		sinzf = std::sin(zf);
		f2 = 0.5 * sinzf * sinzf - 0.25;
		f3 = -0.5 * sinzf * std::cos(zf);

		const double sel = ds_constants.ee2 * f2 + ds_constants.e3 * f3;
		const double sil = ds_constants.xi2 * f2 + ds_constants.xi3 * f3;
		const double sll = ds_constants.xl2 * f2 + ds_constants.xl3 * f3 + ds_constants.xl4 * sinzf;
		const double sghl = ds_constants.xgh2 * f2 + ds_constants.xgh3 * f3 + ds_constants.xgh4 * sinzf;
		const double shl = ds_constants.xh2 * f2 + ds_constants.xh3 * f3;

		const double pe = ses + sel;
		const double pinc = sis + sil;
		const double pl = sls + sll;
		const double pgh = sghs + sghl;
		const double ph = shs + shl;

		xinc += pinc;
		em += pe;

		const double sinis = std::sin(xinc);
		const double cosis = std::cos(xinc);

		if (xinc >= 0.2) {
			omgasm += pgh - cosis * ph / sinis;
			xnodes += ph / sinis;
			xll += pl;
		} else {
			const double sinok = std::sin(xnodes);
			const double cosok = std::cos(xnodes);
			double alfdp = sinis * sinok;
			double betdp = sinis * cosok;
			const double dalf = ph * cosok + pinc * cosis * sinok;
			const double dbet = -ph * sinok + pinc * cosis * cosok;
			alfdp += dalf;
			betdp += dbet;
			xnodes = AngleHelper::wrapRadian(xnodes);
			double xls = xll + omgasm + cosis * xnodes;
			double dls = pl + pgh - pinc * xnodes * sinis;
			xls += dls;
			const double oldxnodes = xnodes;
			xnodes = std::atan2(alfdp, betdp);

			if (std::fabs(oldxnodes - xnodes) > constant::pi) {
				if (xnodes < oldxnodes) {
					xnodes += constant::pi2;
				} else {
					xnodes -= constant::pi2;
				}
			}

			xll += pl;
			omgasm = xls - xll - cosis * xnodes;
		}
	}

	auto propagateSdp4(const double t_min) -> CartesianOrbitalElements {
		double e;
		double a;
		double omega;
		double xl;
		double xnode;
		double xinc;

		double xmdf = m_elements.mean_anomaly + m_common_constants.xmdot * t_min;
		double omgadf = m_elements.argument_perigee + m_common_constants.omgdot * t_min;
		const double xnoddf = m_elements.ascending_node + m_common_constants.xnodot * t_min;

		const double tsq = t_min * t_min;
		xnode = xnoddf + m_common_constants.xnodcf * tsq;
		double tempa = 1.0 - m_common_constants.c1 * t_min;
		double tempe = m_elements.b_star * m_common_constants.c4 * t_min;
		double templ = m_common_constants.t2cof * tsq;

		double xn = m_elements.recovered_mean_motion;
		double em = m_elements.eccentricity;
		xinc = m_elements.inclination;

		deepSpaceSecular(t_min, m_elements, m_common_constants, m_deep_space_constants, m_integrator_params, xmdf, omgadf, xnode, em, xinc,
						 xn);

		if (xn <= 0.0) {
			throw OrbitException("Error: (xn <= 0.0)", OrbitException::ParameterOutOfRange);
		}

		a = pow(constant::xke / xn, constant::tow_third) * tempa * tempa;
		e = em - tempe;
		double xmam = xmdf + m_elements.recovered_mean_motion * templ;

		deepSpacePeriodics(t_min, m_deep_space_constants, e, xinc, omgadf, xnode, xmam);

		if (xinc < 0.0) {
			xinc = -xinc;
			xnode += constant::pi;
			omgadf -= constant::pi;
		}

		xl = xmam + omgadf + xnode;
		omega = omgadf;

		if (e <= -0.001) {
			throw OrbitException("Error: (e <= -0.001)", OrbitException::ParameterOutOfRange);
		} else if (e < 1.0e-6) {
			e = 1.0e-6;
		} else if (e > (1.0 - 1.0e-6)) {
			e = 1.0 - 1.0e-6;
		}

		double perturbed_sinio;
		double perturbed_cosio;
		double perturbed_x3thm1;
		double perturbed_x1mth2;
		double perturbed_x7thm1;
		double perturbed_xlcof;
		double perturbed_aycof;

		setConstantParameters(xinc, perturbed_sinio, perturbed_cosio, perturbed_x3thm1, perturbed_x1mth2, perturbed_x7thm1, perturbed_xlcof,
							  perturbed_aycof);

		return calclateCartesianOrbitalElements(m_elements.epoch.addMinutes(t_min), e, a, omega, xl, xnode, xinc, perturbed_xlcof,
												perturbed_aycof, perturbed_x3thm1, perturbed_x1mth2, perturbed_x7thm1, perturbed_cosio,
												perturbed_sinio);
	}

	auto propagateSgp4(const double t_min) -> CartesianOrbitalElements {
		double e;
		double a;
		double omega;
		double xl;
		double xnode;
		const double xinc = m_elements.inclination;

		const double xmdf = m_elements.mean_anomaly + m_common_constants.xmdot * t_min;
		const double omgadf = m_elements.argument_perigee + m_common_constants.omgdot * t_min;
		const double xnoddf = m_elements.ascending_node + m_common_constants.xnodot * t_min;

		omega = omgadf;
		double xmp = xmdf;

		const double tsq = t_min * t_min;
		xnode = xnoddf + m_common_constants.xnodcf * tsq;
		double tempa = 1.0 - m_common_constants.c1 * t_min;
		double tempe = m_elements.b_star * m_common_constants.c4 * t_min;
		double templ = m_common_constants.t2cof * tsq;

		if (!m_is_using_simple_model) {
			const double delomg = m_near_space_constants.omgcof * t_min;
			const double delm =
			  m_near_space_constants.xmcof * (std::pow(1.0 + m_common_constants.eta * std::cos(xmdf), 3.0) - m_near_space_constants.delmo);
			const double temp = delomg + delm;

			xmp += temp;
			omega -= temp;

			const double tcube = tsq * t_min;
			const double tfour = t_min * tcube;

			tempa = tempa - m_near_space_constants.d2 * tsq - m_near_space_constants.d3 * tcube - m_near_space_constants.d4 * tfour;
			tempe += m_elements.b_star * m_near_space_constants.c5 * (std::sin(xmp) - m_near_space_constants.sinmo);
			templ += m_near_space_constants.t3cof * tcube + tfour * (m_near_space_constants.t4cof + t_min * m_near_space_constants.t5cof);
		}

		a = m_elements.recovered_semi_major_axis * tempa * tempa;
		e = m_elements.eccentricity - tempe;
		xl = xmp + omega + xnode + m_elements.recovered_mean_motion * templ;

		if (e <= -0.001) {
			throw OrbitException("Eccentricity is out of range", OrbitException::EccentricityOutOfRange);
		} else if (e < 1.0e-6) {
			e = 1.0e-6;
		} else if (e > (1.0 - 1.0e-6)) {
			e = 1.0 - 1.0e-6;
		}

		return calclateCartesianOrbitalElements(m_elements.epoch.addMinutes(t_min), e, a, omega, xl, xnode, xinc, m_common_constants.xlcof,
												m_common_constants.aycof, m_common_constants.x3thm1, m_common_constants.x1mth2,
												m_common_constants.x7thm1, m_common_constants.cosio, m_common_constants.sinio);
	}
};

SATFIND_NAMESPACE_END