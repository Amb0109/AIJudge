-- MySQL dump 10.13  Distrib 5.5.17, for Win32 (x86)
--
-- Host: localhost    Database: game
-- ------------------------------------------------------
-- Server version	5.5.17

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `exercise_user`
--

DROP TABLE IF EXISTS `exercise_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `exercise_user` (
  `userid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(20) NOT NULL,
  `userpass` varchar(32) NOT NULL,
  `nickname` varchar(64) DEFAULT NULL,
  `birthday` int(11) DEFAULT '0',
  `sex` tinyint(1) NOT NULL DEFAULT '0',
  `email` varchar(48) DEFAULT NULL,
  `country` int(4) NOT NULL DEFAULT '1',
  `school` varchar(64) DEFAULT NULL,
  `motto` varchar(255) DEFAULT NULL,
  `submits` int(10) unsigned NOT NULL DEFAULT '0',
  `accepts` int(10) unsigned NOT NULL DEFAULT '0',
  `regtime` int(11) NOT NULL,
  `hiddeninfo` int(11) NOT NULL DEFAULT '0',
  `defaultset` tinyint(2) NOT NULL,
  PRIMARY KEY (`userid`),
  UNIQUE KEY `idx_username` (`username`),
  KEY `idx_userac` (`accepts`,`submits`,`regtime`)
) ENGINE=MyISAM AUTO_INCREMENT=277662 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gs_games`
--

DROP TABLE IF EXISTS `gs_games`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gs_games` (
  `game_id` int(11) NOT NULL AUTO_INCREMENT,
  `game_name` varchar(255) NOT NULL,
  `judger_path` varchar(260) NOT NULL,
  `limit_time` int(11) DEFAULT NULL,
  `limit_memory` int(11) DEFAULT NULL,
  `limit_time_onecase` int(11) DEFAULT NULL,
  `tried` int(11) NOT NULL,
  `faq` text,
  PRIMARY KEY (`game_id`)
) ENGINE=MyISAM AUTO_INCREMENT=1002 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gs_source`
--

DROP TABLE IF EXISTS `gs_source`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gs_source` (
  `run_id` int(11) NOT NULL AUTO_INCREMENT,
  `src_code` mediumtext NOT NULL,
  PRIMARY KEY (`run_id`)
) ENGINE=MyISAM AUTO_INCREMENT=193 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gs_status`
--

DROP TABLE IF EXISTS `gs_status`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gs_status` (
  `run_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL DEFAULT '0',
  `game_id` int(11) NOT NULL,
  `submittime` int(11) NOT NULL,
  `language` int(11) NOT NULL,
  `state` int(11) NOT NULL DEFAULT '0',
  `score` int(11) NOT NULL DEFAULT '0',
  `fetched` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`run_id`),
  KEY `fk_submition_game_idx` (`game_id`)
) ENGINE=MyISAM AUTO_INCREMENT=193 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gs_user`
--

DROP TABLE IF EXISTS `gs_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gs_user` (
  `user_id` int(11) NOT NULL,
  `game_id` int(11) NOT NULL,
  `tried` int(11) NOT NULL,
  `score` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`user_id`,`game_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping routines for database 'game'
--
/*!50003 DROP PROCEDURE IF EXISTS `fetch_new_submit` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `fetch_new_submit`()
BEGIN
     DECLARE new_submit_id INT;
     SELECT run_id INTO new_submit_id FROM gs_status WHERE fetched = '0' ORDER BY run_id LIMIT 1;
     UPDATE gs_status SET fetched = '1' WHERE run_id = new_submit_id;
     SELECT run_id,language FROM gs_status WHERE run_id = new_submit_id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `gs_submit` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `gs_submit`(
       p_userid                   int,       
       p_gameid                   int,
       p_submittime               int,
       p_language                 tinyint,
       p_code                     mediumtext CHARACTER SET utf8
)
BEGIN
     DECLARE stmt                 mediumtext;
     DECLARE v_userid             int;

     SET @v_query = CONCAT('INSERT INTO ',
         'gs_status',
         '(user_id,game_id,submittime,language) VALUES(',
         p_userid, ',',
         p_gameid, ',',
         p_submittime, ',',
         p_language,
         ');');
         
     PREPARE stmt from @v_query;
     EXECUTE stmt;
     DEALLOCATE PREPARE stmt;

     
     SET @v_query = CONCAT('INSERT INTO ',
         'gs_source',
         '(src_code) VALUES(',
         '''', p_code ,'''',         
         ');' );
         
     PREPARE stmt from @v_query;
     EXECUTE stmt;
     DEALLOCATE PREPARE stmt;     

     SET @v_query = CONCAT('UPDATE gs_games SET tried = tried + 1 WHERE game_id = ',
          p_gameid );
         
     PREPARE stmt from @v_query;
     EXECUTE stmt;
     DEALLOCATE PREPARE stmt; 


     SELECT COUNT(*) INTO v_userid FROM gs_user WHERE user_id = p_userid AND game_id = p_gameid;
     IF( v_userid <= 0) THEN
        SET @v_query = CONCAT('INSERT INTO ',
         'gs_user',
         '(user_id, game_id, tried, score) VALUES(',
          p_userid,', ', 
          p_gameid ,',1,0', 
         ');' );
         
         PREPARE stmt from @v_query;
         EXECUTE stmt;
         DEALLOCATE PREPARE stmt;
     END IF;     

     IF( v_userid > 0) THEN
        SET @v_query = CONCAT('UPDATE gs_user SET tried = tried + 1 WHERE user_id = ',
          p_userid,' AND game_id = ', p_gameid );
         
         PREPARE stmt from @v_query;
         EXECUTE stmt;
         DEALLOCATE PREPARE stmt;
     END IF;     
     
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-04-05 15:06:42
