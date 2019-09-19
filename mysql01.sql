/*
 Navicat Premium Data Transfer

 Source Server         : mysql
 Source Server Type    : MySQL
 Source Server Version : 80017
 Source Host           : localhost:3306
 Source Schema         : mysql01

 Target Server Type    : MySQL
 Target Server Version : 80017
 File Encoding         : 65001

 Date: 19/09/2019 21:22:58
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for chat_info
-- ----------------------------
DROP TABLE IF EXISTS `chat_info`;
CREATE TABLE `chat_info`  (
  `chat_id` int(11) NOT NULL AUTO_INCREMENT,
  `chat_time` datetime(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6),
  `chat_from` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `chat_to` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `chat_content` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`chat_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for friend_info
-- ----------------------------
DROP TABLE IF EXISTS `friend_info`;
CREATE TABLE `friend_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `me` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `my_friend` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `me`(`me`, `my_friend`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 22 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of friend_info
-- ----------------------------
INSERT INTO `friend_info` VALUES (26, '哈哈', '小白');
INSERT INTO `friend_info` VALUES (10, '哈哈', '小红');
INSERT INTO `friend_info` VALUES (29, '哈哈', '小黑');
INSERT INTO `friend_info` VALUES (22, '小白', '小花');
INSERT INTO `friend_info` VALUES (18, '小白', '小黑');
INSERT INTO `friend_info` VALUES (11, '小红', '哈哈');
INSERT INTO `friend_info` VALUES (24, '小红', '小花');
INSERT INTO `friend_info` VALUES (16, '小红', '小黑');
INSERT INTO `friend_info` VALUES (25, '小花', '小红');
INSERT INTO `friend_info` VALUES (20, '小花', '小黑');
INSERT INTO `friend_info` VALUES (28, '小黑', '哈哈');
INSERT INTO `friend_info` VALUES (19, '小黑', '小白');
INSERT INTO `friend_info` VALUES (14, '小黑', '小红');

-- ----------------------------
-- Table structure for msg_info
-- ----------------------------
DROP TABLE IF EXISTS `msg_info`;
CREATE TABLE `msg_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `msg` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 9 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of msg_info
-- ----------------------------
INSERT INTO `msg_info` VALUES (15, '小白', '小红 申请添加您为好友');
INSERT INTO `msg_info` VALUES (16, '哈哈', '小花 申请添加您为好友');
INSERT INTO `msg_info` VALUES (17, '小花', 'test 申请添加您为好友');

-- ----------------------------
-- Table structure for room_info
-- ----------------------------
DROP TABLE IF EXISTS `room_info`;
CREATE TABLE `room_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `m_member` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `room_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `member_status` int(11) NOT NULL COMMENT '1表示创建者，0表示普通成员',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 223 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for rooms_info
-- ----------------------------
DROP TABLE IF EXISTS `rooms_info`;
CREATE TABLE `rooms_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `room_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `room_count` int(11) NOT NULL,
  `room_creator` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `room_name`(`room_name`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 26 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of rooms_info
-- ----------------------------
INSERT INTO `rooms_info` VALUES (71, '花花世界', 0, '小花');

-- ----------------------------
-- Table structure for user_info
-- ----------------------------
DROP TABLE IF EXISTS `user_info`;
CREATE TABLE `user_info`  (
  `user_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `user_password` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `user_sex` enum('男','女','保密') CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '保密',
  `regist_date` datetime(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6),
  `is_online` tinyint(1) NOT NULL DEFAULT 0,
  `user_grade` int(50) NOT NULL DEFAULT 0,
  PRIMARY KEY (`user_id`) USING BTREE,
  UNIQUE INDEX `user_name`(`user_name`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 8 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_info
-- ----------------------------
INSERT INTO `user_info` VALUES (2, '小红', 'e10adc3949ba59abbe56e057f20f883e', '保密', '2019-09-19 21:05:02.305567', 0, 2);
INSERT INTO `user_info` VALUES (4, '小白', 'e10adc3949ba59abbe56e057f20f883e', '保密', '2019-09-19 21:09:19.208916', 0, 0);
INSERT INTO `user_info` VALUES (5, '小黑', 'e10adc3949ba59abbe56e057f20f883e', '保密', '2019-09-19 21:04:45.386196', 0, 0);
INSERT INTO `user_info` VALUES (6, '哈哈', 'e10adc3949ba59abbe56e057f20f883e', '保密', '2019-09-19 21:03:20.204449', 0, 0);
INSERT INTO `user_info` VALUES (7, '小花', 'e10adc3949ba59abbe56e057f20f883e', '保密', '2019-09-19 21:08:48.664808', 0, 0);
INSERT INTO `user_info` VALUES (8, 'test', 'e10adc3949ba59abbe56e057f20f883e', '保密', '2019-09-19 21:00:45.123889', 0, 0);

SET FOREIGN_KEY_CHECKS = 1;
