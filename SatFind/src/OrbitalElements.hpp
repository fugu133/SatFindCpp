/**
 * @file OrbitalElements.hpp
 * @author fugu133 (@fugumarine)
 * @brief
 * @version 0.1
 * @date 2023-12-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "AngleHelper.hpp"
#include "DateTime.hpp"
#include "Eigen/Geometry"
#include "Essential.hpp"
#include "Tle.hpp"

SATFIND_NAMESPACE_BEGIN

struct OrbitalElements;
struct KeplerianOrbitalElements;
struct CartesianOrbitalElements;

struct OrbitalElements {
	DateTime epoch;
	double eccentricity;
	double inclination;
	double period;
	double perigee;
	double argument_perigee;
	double mean_anomaly;
	double mean_motion;
	double ascending_node;
	double recovered_semi_major_axis;
	double recovered_mean_motion;
	double b_star;

	OrbitalElements()
	  : epoch(),
		eccentricity(0.0),
		inclination(0.0),
		period(0.0),
		perigee(0.0),
		argument_perigee(0.0),
		mean_anomaly(0.0),
		mean_motion(0.0),
		ascending_node(0.0),
		recovered_semi_major_axis(0.0),
		recovered_mean_motion(0.0),
		b_star(0.0) {}

	OrbitalElements(const Tle& tle) : OrbitalElements() { fromTle(tle); }

  private:
	void fromTle(const Tle& tle) {
		// TLEから抽出
		{
			mean_anomaly = AngleHelper::degreeToRadian(tle.meanAnomaly());
			ascending_node = AngleHelper::degreeToRadian(tle.rightAscendingNode());
			argument_perigee = AngleHelper::degreeToRadian(tle.argumentPerigee());
			eccentricity = tle.eccentricity();
			inclination = AngleHelper::degreeToRadian(tle.inclination());
			mean_motion = tle.meanMotion() * constant::pi2 / constant::minutes_per_day;
			b_star = tle.bStar();
			epoch = tle.epoch();
		}
		
		// original mean motion (xnodp), semimajor axis (aodp)
		const double a1 = std::pow(constant::xke / mean_motion, constant::tow_third);
		const double cosio = std::cos(inclination);
		const double theta2 = cosio * cosio;
		const double x3thm1 = 3.0 * theta2 - 1.0;
		const double eosq = eccentricity * eccentricity;
		const double betao2 = 1.0 - eosq;
		const double betao = std::sqrt(betao2);
		const double temp = (1.5 * constant::ck2) * x3thm1 / (betao * betao2);
		const double del1 = temp / (a1 * a1);
		const double a0 = a1 * (1.0 - del1 * (1.0 / 3.0 + del1 * (1.0 + del1 * 134.0 / 81.0)));
		const double del0 = temp / (a0 * a0);

		recovered_mean_motion = mean_motion / (1.0 + del0);
		recovered_semi_major_axis = a0 / (1.0 - del0);
		perigee = (recovered_semi_major_axis * (1.0 - eccentricity) - constant::ae) * constant::xkmper;
		period = constant::pi2 / recovered_mean_motion;
	}
};

struct KeplerianOrbitalElements {
	DateTime epoch;
	double semi_major_axis;
	Angle mean_motion;
	double eccentricity;
	Angle inclination;
	Angle right_ascension_of_ascending_node;
	Angle argument_of_perigee;
	Angle argument_of_latitude;
	Angle mean_anomaly;
	Angle true_anomaly;

	CartesianOrbitalElements toCartesianOrbitalElements() const;

	void fromCartesianOrbitalElements(const CartesianOrbitalElements& e);

	auto toString() const -> std::string {

		std::stringstream ss;
		ss << "Epoch: " << epoch.toString() << "\n";
		ss << "Semi-major axis: " << semi_major_axis << " [m]"
		   << "\n";
		ss << "Mean motion: " << mean_motion.degrees() << " [deg/s]"
		   << "\n";
		ss << "Eccentricity: " << eccentricity << "\n";
		ss << "Inclination: " << inclination.degrees() << " [deg]"
		   << "\n";
		ss << "RAAN: " << right_ascension_of_ascending_node.degrees() << " [deg]"
		   << "\n";
		ss << "Argument of perigee: " << argument_of_perigee.degrees() << " [deg]"
		   << "\n";
		ss << "Argument of latitude: " << argument_of_latitude.degrees() << " [deg]"
		   << "\n";
		ss << "Mean anomaly: " << mean_anomaly.degrees() << " [deg]";

		return ss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const KeplerianOrbitalElements& e) {
		os << e.toString();
		return os;
	}
};

struct CartesianOrbitalElements {
	DateTime epoch;
	Eci position; // [m]
	Eci velocity; // [m/s]

	auto toString() const -> std::string {

		std::stringstream ss;
		ss << "Epoch: " << epoch.toString() << "\n";
		ss << "Position: " << position.elements().transpose() << " [m]"
		   << "\n";
		ss << "Velocity: " << velocity.elements().transpose() << " [m/s]";

		return ss.str();
	}

	KeplerianOrbitalElements toKeplerianOrbitalElements() const;

	void fromKeplerianOrbitalElements(const KeplerianOrbitalElements& e);

	friend std::ostream& operator<<(std::ostream& os, const CartesianOrbitalElements& e) {
		os << e.toString();
		return os;
	}
};

void KeplerianOrbitalElements::fromCartesianOrbitalElements(const CartesianOrbitalElements& e) {
	*this = e.toKeplerianOrbitalElements();
}

CartesianOrbitalElements KeplerianOrbitalElements::toCartesianOrbitalElements() const {
	CartesianOrbitalElements c_e;

	// Calculate the eccentric anomaly from Kepler equation
	double E0 = mean_anomaly.radians() + eccentricity * sin(mean_anomaly.radians());
	double E = 0;
	double delta_E = 1e3;
	for (int i = 0; i < 10 || std::abs(delta_E) > 1e-15; i++) {
		delta_E = (E0 - eccentricity * sin(E0) - mean_anomaly.radians()) / (1 - eccentricity * std::cos(E0));
		E = E0 - delta_E;
		E0 = E;
	}

	// Calculate the position in the perifocal frame
	Eigen::Vector3d r_pqw = Eigen::Vector3d::Zero();
	r_pqw.x() = semi_major_axis * (std::cos(E) - eccentricity);
	r_pqw.y() = semi_major_axis * std::sqrt(1.0 - eccentricity * eccentricity) * std::sin(E);

	// Calculate the velocity in the perifocal frame
	const double c = std::sqrt(constant::mu * 1e9 / semi_major_axis) / (1 - eccentricity * std::cos(E));
	Eigen::Vector3d v_pqw = Eigen::Vector3d::Zero();
	v_pqw.x() = -c * std::sin(E);
	v_pqw.y() = c * std::sqrt(1.0 - eccentricity * eccentricity) * std::cos(E);

	// Calculate the rotation matrix from the perifocal frame to the ECI frame
	Eigen::Matrix3d R_pqw_to_eci = Eigen::Matrix3d::Zero();
	const double cos_raan = right_ascension_of_ascending_node.cos();
	const double sin_raan = right_ascension_of_ascending_node.sin();
	const double cos_arg_per = argument_of_perigee.cos();
	const double sin_arg_per = argument_of_perigee.sin();
	const double cos_incl = inclination.cos();
	const double sin_incl = inclination.sin();
	R_pqw_to_eci(0, 0) = cos_raan * cos_arg_per - sin_raan * sin_arg_per * cos_incl;
	R_pqw_to_eci(0, 1) = -cos_raan * sin_arg_per - sin_raan * cos_arg_per * cos_incl;
	R_pqw_to_eci(0, 2) = sin_raan * sin_incl;
	R_pqw_to_eci(1, 0) = sin_raan * cos_arg_per + cos_raan * sin_arg_per * cos_incl;
	R_pqw_to_eci(1, 1) = -sin_raan * sin_arg_per + cos_raan * cos_arg_per * cos_incl;
	R_pqw_to_eci(1, 2) = -cos_raan * sin_incl;
	R_pqw_to_eci(2, 0) = sin_arg_per * sin_incl;
	R_pqw_to_eci(2, 1) = cos_arg_per * sin_incl;
	R_pqw_to_eci(2, 2) = cos_incl;

	// Copy the epoch
	c_e.epoch = epoch;

	// Conversion position to the ECI frame
	c_e.position = Eci(epoch, R_pqw_to_eci * r_pqw);

	// Conversion velocity to the ECI frame
	c_e.velocity = Eci(epoch, R_pqw_to_eci * v_pqw);

	return c_e;
}

void CartesianOrbitalElements::fromKeplerianOrbitalElements(const KeplerianOrbitalElements& e) {
	*this = e.toCartesianOrbitalElements();
}

KeplerianOrbitalElements CartesianOrbitalElements::toKeplerianOrbitalElements() const {
	KeplerianOrbitalElements k_e;
	k_e.epoch = epoch;													// epoch
	Eigen::Vector3d h = position.elements().cross(velocity.elements()); // orbital momentum vector [m^2/s]
	Eigen::Vector3d n(-h.y(), h.x(), 0);								// node line vector [m^2/s]
	Eigen::Vector3d e = velocity.elements().cross(h) / (constant::mu * 1e9) - position.elements().normalized(); // eccentricity vector
	k_e.eccentricity = e.norm();
	k_e.inclination = Radian{acos(h.z() / h.norm())}; // inclination [rad]

	k_e.true_anomaly = Radian{acos(e.dot(position.elements()) / (e.norm() * position.elements().norm()))}; // true anomaly [rad]
	if (position.elements().dot(velocity.elements()) < 0) {
		k_e.true_anomaly = Radian{constant::pi2} - k_e.true_anomaly;
	}

	k_e.argument_of_latitude =
	  Radian{acos(n.dot(position.elements()) / (n.norm() * position.elements().norm()))}; // argument of latitude [rad]
	if (position.z() < 0) {
		k_e.argument_of_latitude = Radian{constant::pi2} - k_e.argument_of_latitude;
	}

	double E =
	  2 * atan(sqrt((1 - k_e.eccentricity) / (1 + k_e.eccentricity)) * tan(k_e.true_anomaly.radians() / 2)); // eccentric anomaly [rad]
	k_e.mean_anomaly = Radian{AngleHelper::wrapRadian(E - k_e.eccentricity * sin(E))};						 // mean anomaly [rad]

	k_e.argument_of_perigee = Radian{acos(n.dot(e) / (n.norm() * e.norm()))}; // argument of perigee [rad]
	if (e.z() < 0) {
		k_e.argument_of_perigee = Radian{constant::pi2} - k_e.argument_of_perigee;
	}

	k_e.right_ascension_of_ascending_node = Radian{acos(n.x() / n.norm())}; // right ascension of ascending node [rad]
	if (n.y() < 0) {
		k_e.right_ascension_of_ascending_node = Radian{constant::pi2} - k_e.right_ascension_of_ascending_node;
	}

	k_e.semi_major_axis = pow(h.norm(), 2) / (constant::mu * 1e9 * (1 - pow(k_e.eccentricity, 2))); // semi-major axis [m]

	k_e.mean_motion = Radian{sqrt(constant::mu * 1e9 / pow(k_e.semi_major_axis, 3))}; // mean motion [rad/s]

	return k_e;
}

SATFIND_NAMESPACE_END