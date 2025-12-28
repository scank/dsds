#!/usr/bin/env python
import rospy

import actionlib
import roslaunch
from actionlib_msgs.msg import *
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from nav_msgs.msg import Path
from std_msgs.msg import String
from geometry_msgs.msg import PoseWithCovarianceStamped
from tf_conversions import transformations
from xml.dom.minidom import parse
from math import pi
import tf

class navigation_demo:
    def __init__(self):
        self.set_pose_pub = rospy.Publisher('/initialpose', PoseWithCovarianceStamped, queue_size=5)

        self.move_base = actionlib.SimpleActionClient("move_base", MoveBaseAction)
        self.move_base.wait_for_server(rospy.Duration(60))
        
        
        self.tf_listener = tf.TransformListener()
        self.get_point = rospy.Publisher('get_pos', String, queue_size=5)
        
        self.plist = []
        self.success_count = 0
        
    def set_plist(self,plist):
        self.plist = plist
            
    def set_pose(self, p):
        if self.move_base is None:
            return False

        x, y, th = p

        pose = PoseWithCovarianceStamped()
        pose.header.stamp = rospy.Time.now()
        pose.header.frame_id = 'map'
        pose.pose.pose.position.x = x
        pose.pose.pose.position.y = y
        q = transformations.quaternion_from_euler(0.0, 0.0, th/180.0*pi)
        pose.pose.pose.orientation.x = q[0]
        pose.pose.pose.orientation.y = q[1]
        pose.pose.pose.orientation.z = q[2]
        pose.pose.pose.orientation.w = q[3]

        self.set_pose_pub.publish(pose)
        return True

    def _done_cb(self, status, result):
        rospy.loginfo("navigation done! status:%d result:%s"%(status, result))

    def _active_cb(self):
        rospy.loginfo("[Navi] navigation has be actived")

    def _feedback_cb(self, feedback):
        rospy.loginfo("[Navi] navigation feedback\r\n%s"%feedback)
        
    def goto(self, p):
        goal = MoveBaseGoal()

        goal.target_pose.header.frame_id = 'map'
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = p[0]
        goal.target_pose.pose.position.y = p[1]
        goal.target_pose.pose.position.z = p[2]
        #q = transformations.quaternion_from_euler(0.0, 0.0, p[2]/180.0*pi)
        goal.target_pose.pose.orientation.x = p[3]
        goal.target_pose.pose.orientation.y = p[4]
        goal.target_pose.pose.orientation.z = p[5]
        goal.target_pose.pose.orientation.w = p[6]

        self.move_base.send_goal(goal, self._done_cb, self._active_cb, self._feedback_cb)
        
        result = self.move_base.wait_for_result(rospy.Duration(60))
        print(result)
       
        state = self.move_base.get_state()
        if state == GoalStatus.SUCCEEDED:
            self.success_count += 1
            if len(self.plist) == self.success_count:
                rospy.loginfo("arrived goal point")
                self.get_point.publish("1")
                self.isSendVoice = False
        return True

    def cancel(self):
        self.move_base.cancel_all_goals()
        return True
    

def callback(msg):
    
    doc = parse("/home/bcsh/waypoints.xml")
    root_element = doc.documentElement
    points = root_element.getElementsByTagName("Waypoint")
    
    plist = []
    
    rospy.loginfo("set pose...")
    navi = navigation_demo()
    
    for p in points:
        point = [0] * 7
        point[0] = float(p.getElementsByTagName("Pos_x")[0].childNodes[0].data)
        point[1] = float(p.getElementsByTagName("Pos_y")[0].childNodes[0].data)
        point[2] = float(p.getElementsByTagName("Pos_z")[0].childNodes[0].data)
        point[3] = float(p.getElementsByTagName("Ori_x")[0].childNodes[0].data)
        point[4] = float(p.getElementsByTagName("Ori_y")[0].childNodes[0].data)
        point[5] = float(p.getElementsByTagName("Ori_z")[0].childNodes[0].data)
        point[6] = float(p.getElementsByTagName("Ori_w")[0].childNodes[0].data)
        plist.append(point)
        
    print(plist)
    
    rospy.loginfo("goto goal...")
    navi.set_plist(plist)
    
    for waypoint in plist:
        #print(waypoint)
        navi.goto(waypoint)
    
    
if __name__ == "__main__":
    rospy.init_node('auto_slam_node',anonymous=True)
    rospy.Subscriber("auto_slam", String,callback)
    
    rospy.spin()
    r = rospy.Rate(0.2)
    r.sleep()
    
    




