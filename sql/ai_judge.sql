# Host: Localhost  (Version: 5.5.17)
# Date: 2013-04-29 15:07:17
# Generator: MySQL-Front 5.3  (Build 2.52)

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE */;
/*!40101 SET SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES */;
/*!40103 SET SQL_NOTES='ON' */;

DROP DATABASE IF EXISTS `ai_judge_server`;
CREATE DATABASE `ai_judge_server` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `ai_judge_server`;

#
# Source for table "file_judger_exec"
#

DROP TABLE IF EXISTS `file_judger_exec`;
CREATE TABLE `file_judger_exec` (
  `judgerId` int(11) NOT NULL AUTO_INCREMENT,
  `judgerVersion` int(11) DEFAULT '0',
  `judgerCreated` int(11) DEFAULT '0',
  `execPath` varchar(255) NOT NULL DEFAULT '',
  `execCheck` varchar(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`judgerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='判定程序存放文件库';

#
# Data for table "file_judger_exec"
#


#
# Source for table "file_user_exec"
#

DROP TABLE IF EXISTS `file_user_exec`;
CREATE TABLE `file_user_exec` (
  `runId` int(11) NOT NULL DEFAULT '0',
  `execPath` varchar(255) NOT NULL DEFAULT '',
  `execCheck` varchar(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`runId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='用户可执行程序存放文件库';

#
# Data for table "file_user_exec"
#


#
# Source for table "web_game"
#

DROP TABLE IF EXISTS `web_game`;
CREATE TABLE `web_game` (
  `gameId` int(11) NOT NULL AUTO_INCREMENT,
  `gameName` varchar(64) NOT NULL DEFAULT '',
  `projId` int(11) NOT NULL DEFAULT '0',
  `gameInfo` mediumtext,
  `gameFaq` mediumtext,
  `judgerId` int(11) NOT NULL DEFAULT '0',
  `judgerCfg` mediumtext NOT NULL,
  PRIMARY KEY (`gameId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='游戏，判定程序是以这个为单位的';

#
# Data for table "web_game"
#


#
# Source for table "web_match"
#

DROP TABLE IF EXISTS `web_match`;
CREATE TABLE `web_match` (
  `matchId` int(11) NOT NULL AUTO_INCREMENT,
  `gameId` int(11) NOT NULL DEFAULT '0',
  `matchInfo` mediumtext NOT NULL,
  `judgeRes` mediumtext NOT NULL,
  `fetched` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`matchId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='* 理想的match表，完全自由配置';

#
# Data for table "web_match"
#


#
# Source for table "web_match_d"
#

DROP TABLE IF EXISTS `web_match_d`;
CREATE TABLE `web_match_d` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='双人游戏match表';

#
# Data for table "web_match_d"
#


#
# Source for table "web_match_s"
#

DROP TABLE IF EXISTS `web_match_s`;
CREATE TABLE `web_match_s` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='单人游戏match表';

#
# Data for table "web_match_s"
#


#
# Source for table "web_project"
#

DROP TABLE IF EXISTS `web_project`;
CREATE TABLE `web_project` (
  `projId` int(11) NOT NULL AUTO_INCREMENT,
  `projName` varchar(64) NOT NULL DEFAULT '',
  `projInfo` mediumtext,
  `projFaq` mediumtext,
  PRIMARY KEY (`projId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='* 把同类的game放在一个项目里面';

#
# Data for table "web_project"
#


#
# Source for table "web_source"
#

DROP TABLE IF EXISTS `web_source`;
CREATE TABLE `web_source` (
  `runId` int(11) NOT NULL DEFAULT '0',
  `srcCode` mediumtext NOT NULL,
  `srcCheck` varchar(64) NOT NULL DEFAULT '',
  `compileRes` smallint(6) NOT NULL DEFAULT '0',
  `compileInfo` mediumtext,
  PRIMARY KEY (`runId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='用户提交源代码表，包含编译信息';

#
# Data for table "web_source"
#


#
# Source for table "web_status"
#

DROP TABLE IF EXISTS `web_status`;
CREATE TABLE `web_status` (
  `userId` int(11) NOT NULL DEFAULT '0',
  `gameId` int(11) NOT NULL DEFAULT '0',
  `submitCnt` int(11) NOT NULL DEFAULT '0',
  `score` decimal(10,2) NOT NULL DEFAULT '0.00',
  PRIMARY KEY (`userId`,`gameId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='用户 - 游戏 的分数信息';

#
# Data for table "web_status"
#


#
# Source for table "web_submition"
#

DROP TABLE IF EXISTS `web_submition`;
CREATE TABLE `web_submition` (
  `runId` int(11) NOT NULL AUTO_INCREMENT,
  `userId` int(11) NOT NULL DEFAULT '0',
  `gameId` int(11) NOT NULL DEFAULT '0',
  `language` smallint(3) NOT NULL DEFAULT '0',
  `submitData` int(11) NOT NULL DEFAULT '0',
  `fetched` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`runId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='用户提交表';

#
# Data for table "web_submition"
#


#
# Source for table "web_user"
#

DROP TABLE IF EXISTS `web_user`;
CREATE TABLE `web_user` (
  `userId` int(11) NOT NULL AUTO_INCREMENT,
  `userName` varchar(20) NOT NULL DEFAULT '',
  `userPass` varchar(32) NOT NULL DEFAULT '',
  `nickName` varchar(64) DEFAULT NULL,
  `regData` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`userId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='用户表';

#
# Data for table "web_user"
#


/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
