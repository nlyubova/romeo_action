#ifndef ACTIONS_HPP
#define ACTIONS_HPP

// ROS
#include <ros/ros.h>

// MoveIt!
#include <moveit/move_group_interface/move_group.h>

// Grasp generation
#include <moveit_simple_grasps/simple_grasps.h>
#include <moveit_simple_grasps/grasp_data.h>

//for showing grasps
#include <moveit_visual_tools/moveit_visual_tools.h>

#include <romeo_pick_place/metablock.hpp>
#include <romeo_pick_place/postures.hpp>
#include <romeo_pick_place/grasp_filter.hpp>

namespace romeo_pick_place
{

class Action
{
public:
  Action(ros::NodeHandle *nh_,
         moveit_visual_tools::MoveItVisualToolsPtr &visual_tools,
         const std::string arm_,
         const std::string robot_name);
  bool pickAction(MetaBlock *block, const std::string surface_name);
  bool placeAction(MetaBlock *block);
  bool pickDefault(MetaBlock *block);

  //reaching positions generated by moveit simple grasps
  bool graspPlan(MetaBlock *block, const std::string surface_name);
  bool graspPlanAndMove(MetaBlock *block, const std::string surface_name);
  bool graspPlanAllPossible(MetaBlock *block, const std::string surface_name);
  bool executeAction();

  //reaching default grasping pose
  float reachGrasp(geometry_msgs::Pose pose_target, const std::string surface_name);
  bool reachPregrasp(geometry_msgs::Pose pose_target, const std::string surface_name);
  bool reachAction(geometry_msgs::Pose pose_target, const std::string surface_name="");
  bool reachInitPose();

  void filterGrasps(MetaBlock *block);

  geometry_msgs::Pose getPose();
  bool poseHandInit();
  bool poseHandZero();
  void poseHandOpen();
  void poseHandClose();
  bool poseHeadDown();
  bool poseHeadZero();

  void setTolerance(const double value);

  //active end effector
  const std::string arm;
  const std::string end_eff;
  const std::string plan_group;
  Posture posture;

  //grasp configuration
  moveit_simple_grasps::GraspData grasp_data_;

private:
  std::vector<moveit_msgs::Grasp> generateGrasps(MetaBlock *block);
  std::vector<geometry_msgs::Pose> configureForPlanning(const std::vector<moveit_msgs::Grasp> &grasps);

  //grasp generator
  moveit_simple_grasps::SimpleGraspsPtr simple_grasps_;

  //interface with MoveIt
  boost::scoped_ptr<move_group_interface::MoveGroup> move_group_;

  //for planning actions
  boost::shared_ptr<moveit::planning_interface::MoveGroup::Plan> current_plan_;

  moveit_visual_tools::MoveItVisualToolsPtr visual_tools_;

  ros::Publisher pub_obj_pose, pub_obj_poses;

  bool verbose_;

  geometry_msgs::Pose pose_init;

  // class for filter object
  GraspFilterPtr grasp_filter_;
};



}

#endif // ACTIONS_HPP
