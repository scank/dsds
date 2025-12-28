#!/usr/bin/env python
#-*- coding: utf-8 -*-
import rospy
from actionlib import SimpleActionClient
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from geometry_msgs.msg import PoseStamped, Pose

def main():
    rospy.init_node('hello', anonymous=True)
    client = SimpleActionClient('move_base', MoveBaseAction)
    client.wait_for_server()

    # 目标点列表
    goals = [
        MoveBaseGoal(target_pose=PoseStamped(
            header=rospy.Header(frame_id="map"),
            pose=Pose(position=Point(x=2.6, y=0.05), orientation=Quaternion(z=0.0, w=1.0))
        ))
    ]

    for goal in goals:
        client.send_goal(goal)
        wait = client.wait_for_result()
        if not wait:
            rospy.logerr("Action server not available!")
            break

        if client.get_state() == SimpleActionClient.DONE:
            rospy.loginfo("机器人到达目标点")
        else:
            rospy.logwarn("未成功到达目标点")

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
