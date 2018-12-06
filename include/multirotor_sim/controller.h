#pragma once

#include "Eigen/Core"
#include "Eigen/Geometry"
#include <Eigen/StdVector>

#include "geometry/quat.h"
#include "geometry/support.h"
#include "lin_alg_tools/care.h"

#include "utils.h"
#include "dynamics.h"
#include "pid.h"
#include "nlc.h"

using namespace quat;
using namespace Eigen;

namespace controller
{

class Controller
{

public:

  Controller();
  
  // Waypoint Enumerations
  enum
  {
    PX,
    PY,
    PZ,
    PSI
  };
  
  // PID controllers
  PID<double> roll_;
  PID<double> pitch_;
  PID<double> yaw_rate_;

  // Nonlinear controller
  NLC<double> nlc_;
  Eigen::Matrix3d K_p_; // position
  Eigen::Matrix3d K_v_; // velocity
  Eigen::Matrix3d K_d_; // disturbance acceleration

  // Parameters
  int control_type_;
  double sh_;
  double mass_;
  double max_thrust_;
  double drag_constant_;
  double waypoint_threshold_;
  double waypoint_velocity_threshold_;
  double vmag_;
  int path_type_;
  double traj_heading_walk_;
  double traj_heading_straight_gain_;
  std::default_random_engine rng_;
  std::uniform_real_distribution<double> udist_;
  Eigen::MatrixXd waypoints_;
  int current_waypoint_id_;

  // Hover throttle estimation
  Vector3d vhat_;
  double sh_inv_hat_;
  double s_prev_;
  double sh_kv_;
  double sh_ks_;

  // Circular Trajectory Parameters
  double traj_delta_north_;
  double traj_delta_east_;
  double traj_delta_alt_;
  double traj_delta_yaw_;
  double traj_nom_north_;
  double traj_nom_east_;
  double traj_nom_alt_;
  double traj_nom_yaw_;
  double traj_north_freq_;
  double traj_east_freq_;
  double traj_alt_freq_;
  double traj_yaw_freq_;

  // Memory for sharing information between functions
  bool initialized_;
  state_t xhat_ = {}; // estimate
  state_t xc_ = {}; // command
  max_t max_ = {};
  double prev_time_;

  // Functions
  void load(const std::string filename);
  void updateWaypointManager();
  void updateTrajectoryManager();
  void computeControl(const dynamics::xVector &x, const double t, dynamics::commandVector& u); 
  
  inline state_t getCommandedState() const { return xc_; } 
  const Eigen::MatrixXd get_waypoints() const { return waypoints_; }
};

}
