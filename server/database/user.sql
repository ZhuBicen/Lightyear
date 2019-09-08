/*
SQLyog 企业版 - MySQL GUI v7.14 
MySQL - 5.5.29 : Database - dy_enterprise_stock
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/`dy_enterprise_stock` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `dy_enterprise_stock`;

/*Table structure for table `xt_admin` */

DROP TABLE IF EXISTS `xt_admin`;

CREATE TABLE `xt_admin` (
  `id` varchar(255) NOT NULL COMMENT 'id',
  `adminCode` varchar(255) DEFAULT NULL COMMENT '编码',
  `addTime` datetime DEFAULT NULL COMMENT '注册时间',
  `department` varchar(255) DEFAULT NULL COMMENT '部门',
  `lastLoginTime` datetime DEFAULT NULL COMMENT '上次登录时间',
  `loginIp` varchar(255) DEFAULT NULL COMMENT '上次登录Ip',
  `loginName` varchar(255) DEFAULT NULL COMMENT '登录名',
  `loginTimes` varchar(255) DEFAULT NULL COMMENT '登录次数',
  `password` varchar(255) DEFAULT NULL COMMENT '登录密码',
  `realName` varchar(255) DEFAULT NULL COMMENT '真实姓名',
  `status` varchar(255) DEFAULT NULL COMMENT '状态',
  `telephone` varchar(255) DEFAULT NULL COMMENT '电话',
  `flagTime` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
INSERT INTO `xt_admin` VALUES 
	('0','WW-000',NULL,'制造部',NULL,NULL,NULL,NULL,'123','张晓欣',NULL,'1389999999',NULL),
	('1','WW-001',NULL,'总经理',NULL,NULL,NULL,NULL,'123','同  刚',NULL,'1345666666',NULL),
	('2','WW-002',NULL,'制造部',NULL,NULL,NULL,NULL,'123','李金龙',NULL,'1389999999',NULL),
	('3','WW-003',NULL,'制造部',NULL,NULL,NULL,NULL,'123','杨尔坤',NULL,'1389999999',NULL),
	('4','WW-004',NULL,'制造部',NULL,NULL,NULL,NULL,'123','王红绢',NULL,'1389999999',NULL),
	('5','WW-005',NULL,'制造部',NULL,NULL,NULL,NULL,'123','杨亚明',NULL,'1389999999',NULL),
	('6','WW-006',NULL,'物流部',NULL,NULL,NULL,NULL,'123','任  越',NULL,'1389999999',NULL);
