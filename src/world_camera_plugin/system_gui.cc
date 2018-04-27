/*
* 
*
*camnera control
*
*
**/

#include <sdf/sdf.hh>

#include <gazebo/common/common.hh>
#include <gazebo/gui/GuiIface.hh>
#include "gazebo/physics/physics.hh"
#include <gazebo/rendering/rendering.hh>
#include "gazebo/rendering/Light.hh"
#include "gazebo/transport/transport.hh"
#include "gazebo/msgs/msgs.hh"
#include <gazebo/gazebo.hh>

namespace gazebo
{
  class WordCamera : public WorldPlugin
  {

	public: void Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf)
    {
		this->scene  = rendering::get_scene();
		this->node=transport::NodePtr(new transport::Node());
		this->node->Init(_parent->GetName());
		this->sub=this->node->Subscribe("~/keyboard/keypress",&WordCamera::onMesage,this);
		this->camera_sub=this->node->Subscribe("~/user_camera/pose",&WordCamera::onCameraMessage,this);
		//pub light msg
		this->light_pub=this->node->Advertise<msgs::Light>("~/light/modify");
		this->model=_parent->GetModel("iris_demo");
		//
    }
    /////////////////////////////////////////////
    /// \brief handle keyboard message 
    private: void onMesage(ConstAnyPtr &message)
    {      		
		if(message->int_value()==16777235){

		}	  	
		
		if(message->int_value()==16777237){
				  	
		}
    }
	private:void onCameraMessage(ConstPosePtr &userCamerMsg){
		if(userCamerMsg->has_position()){	
			msgs::Light light_msg;
			this->light =this->scene->GetLight("light_for_vehicle");
			this->light->FillMsg(light_msg);
			ignition::math::Vector3d light_offse(this->model->GetWorldPose().Ign().Pos().X(),this->model->GetWorldPose().Ign().Pos().Y(),this->model->GetWorldPose().Ign().Pos().Z()+5);
			msgs::Set(light_msg.mutable_pose()->mutable_position(),light_offse);
			double model_yaw=this->model->GetWorldPose().Ign().Rot().Euler().Z();
			ignition::math::Quaterniond light_rot(0,-1.5707,model_yaw);
			msgs::Set(light_msg.mutable_pose()->mutable_orientation(),light_rot);
			this->light_pub->Publish(light_msg);
			//gzwarn<< this->model->GetWorldPose().Ign().Pos().X();
		}	
	}

	private: rendering::ScenePtr scene;
	private: transport::NodePtr node;
	private: transport::SubscriberPtr sub;
	private: transport::SubscriberPtr camera_sub;
	private: transport::PublisherPtr  light_pub;
	private: physics:: ModelPtr model;
	private: rendering::LightPtr light;
	
  };

  // Register this plugin with the simulator
  GZ_REGISTER_WORLD_PLUGIN(WordCamera)
}
