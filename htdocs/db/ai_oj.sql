-- phpMyAdmin SQL Dump
-- version 4.0.1
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2013 年 05 月 24 日 10:01
-- 服务器版本: 5.5.17
-- PHP 版本: 5.3.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- 数据库: `ai_oj`
--

DELIMITER $$
--
-- 存储过程
--
CREATE DEFINER=`root`@`localhost` PROCEDURE `fetch_new_match`()
BEGIN
     DECLARE v_matchid             int;
     
     SELECT MIN(matchId) INTO v_matchid FROM web_match WHERE fetched = 0;
     UPDATE web_match SET fetched = 1 WHERE matchId = v_matchid;     
     SELECT matchId, gameId, playerCnt, playerId1, playerId2, playerId3, playerId4, playerId5, playerId6 FROM web_match WHERE matchId = v_matchid; 
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `fetch_new_submition`()
BEGIN
     DECLARE v_runid             int;

     SELECT MIN(runId) INTO v_runid FROM web_submition WHERE fetched = 0;
     UPDATE web_submition SET fetched = 1 WHERE runId = v_runid;     
     SELECT runId, gameId, userId, language FROM web_submition WHERE runId = v_runid; 
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `gs_submit`(
       p_userid                   int,       
       p_gameid                   int,
       p_submittime               int,
       p_language                 tinyint,       
       p_codelength               int,                 
       p_code                     mediumtext
)
BEGIN
     DECLARE stmt                 mediumtext;
     DECLARE v_userid             int;     
     DECLARE v_insertedid         int;

     SET @v_query = CONCAT('INSERT INTO ',
         'web_submition',
         '(userId,gameId,submitDate,language,codeLength) VALUES(',
         p_userid, ',',
         p_gameid, ',',
         p_submittime, ',',
         p_language, ',',         
         p_codelength,
         ');');
         
     PREPARE stmt from @v_query;
     EXECUTE stmt;
     DEALLOCATE PREPARE stmt;

     SELECT LAST_INSERT_ID() INTO v_insertedid;
     SET @v_query = CONCAT('INSERT INTO ',
         'web_source',
         '(runId, srcCode) VALUES(',v_insertedid, ',',
         '''', p_code ,'''',         
         ');' );
         
     PREPARE stmt from @v_query;
     EXECUTE stmt;
     DEALLOCATE PREPARE stmt;

     SET @v_query = CONCAT('UPDATE web_game SET tried = tried + 1 WHERE gameId = ',
          p_gameid );
         
     PREPARE stmt from @v_query;
     EXECUTE stmt;
     DEALLOCATE PREPARE stmt; 


     /*SELECT COUNT(*) INTO v_userid FROM web_user WHERE userId = p_userid AND game_id = p_gameid;
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
     END IF;     */
     
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `store_player_exec_file`(IN `p_runid` INT, IN `p_execPath` VARCHAR(260), IN `p_execMd5` VARCHAR(35))
    NO SQL
BEGIN
    SET @v_query = CONCAT(
    'INSERT INTO file_player_exec(runId, execPath, execMd5) VALUES (', p_runid,',',        
    '''',p_execPath,'''',',','''',p_execMd5,'''',');'
    );
    PREPARE stmt from @v_query;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `upload_compile_result`(IN `p_runid` INT, IN `p_userid` INT, IN `p_gameid` INT, IN `p_result` INT, IN `p_resinfo` MEDIUMTEXT)
BEGIN
    DECLARE stmt				mediumtext;
    DECLARE v_num				int;
	DECLARE v_res				int;

	IF (p_result = 1) THEN
		SET v_res = 2;
	ELSE
		SET v_res = -1;
	END IF;

	INSERT INTO web_cerror (runId, complieInfo) VALUES(p_runid, p_resinfo);
    UPDATE web_submition SET fetched = v_res WHERE runId = p_runid;
    SELECT COUNT(*) INTO v_num FROM web_status WHERE userId = p_userid AND gameId = p_gameid;

     IF (v_num <= 0 AND v_res >= 0) THEN
        SET @v_query = CONCAT(
        'INSERT INTO web_status(userId, gameId, runId) VALUES (',
        p_userid , ',' ,p_gameid , ',', p_runid, ');'
        );
        PREPARE stmt from @v_query;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
     END IF;
                 
     IF (v_num > 0 AND v_res >= 0) THEN
        SET @v_query = CONCAT(
        'UPDATE web_status SET runId = ', p_runid,
        ' WHERE userId = ',p_userid,' AND gameId = ', p_gameid
        );
        PREPARE stmt from @v_query;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
     END IF;   
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `upload_match_result`(
       p_matchid                   int,
       p_judgeres                  mediumtext
)
BEGIN
     DECLARE stmt                 mediumtext;

     SET @v_query = CONCAT('UPDATE web_match SET fetched = 2, judgeRes = ',
         '''', p_judgeres, '''', ' WHERE matchId = ', p_matchid,
         ';');
         
     PREPARE stmt from @v_query;
     EXECUTE stmt;
     DEALLOCATE PREPARE stmt;
     
END$$

DELIMITER ;

-- --------------------------------------------------------

--
-- 表的结构 `file_judger_exec`
--

CREATE TABLE IF NOT EXISTS `file_judger_exec` (
  `gameId` int(11) NOT NULL,
  `execPath` varchar(255) NOT NULL DEFAULT '',
  `execCheck` varchar(64) NOT NULL DEFAULT '',
  `judgeConfig` mediumtext,
  PRIMARY KEY (`gameId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='判定程序存放文件库';

--
-- 转存表中的数据 `file_judger_exec`
--

INSERT INTO `file_judger_exec` (`gameId`, `execPath`, `execCheck`, `judgeConfig`) VALUES
(1000, 'othello.exe', '', '{"time_limit":20000,"memory_limit":65536,"duration_limit":80000}'),
(1001, 'rps.exe', '', '{"time_limit":1000,"memory_limit":65536,"duration_limit":4000}');

-- --------------------------------------------------------

--
-- 表的结构 `file_player_exec`
--

CREATE TABLE IF NOT EXISTS `file_player_exec` (
  `runId` int(11) NOT NULL DEFAULT '0',
  `execPath` varchar(255) NOT NULL DEFAULT '',
  `execMd5` varchar(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`runId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='用户可执行程序存放文件库';

-- --------------------------------------------------------

--
-- 表的结构 `web_cerror`
--

CREATE TABLE IF NOT EXISTS `web_cerror` (
  `runId` int(11) NOT NULL,
  `complieInfo` mediumtext,
  PRIMARY KEY (`runId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- 表的结构 `web_game`
--

CREATE TABLE IF NOT EXISTS `web_game` (
  `gameId` int(11) NOT NULL AUTO_INCREMENT,
  `gameName` varchar(64) NOT NULL DEFAULT '',
  `gameInfo` mediumtext,
  `gameFaq` mediumtext,
  `tried` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`gameId`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='游戏，判定程序是以这个为单位的' AUTO_INCREMENT=1002 ;

--
-- 转存表中的数据 `web_game`
--

INSERT INTO `web_game` (`gameId`, `gameName`, `gameInfo`, `gameFaq`, `tried`) VALUES
(1000, 'Reversi', '<p>黑白棋 Reversi </p>\r\n<p><a href="http://zh.wikipedia.org/wiki/%E9%BB%91%E7%99%BD%E6%A3%8B" target="_blank"> 黑白棋简介 </a></p>\r\n<p><a href="http://www.soongsky.com/computer/" target="_blank"> 黑白棋AI编写教程 </a></p>', '<div>\r\n  <p><b># 游戏玩法</b></p>\r\n  <p>游戏开始时会程序会收到 <code>first</code> 或者 <code>second</code> 的输入，告诉你是游戏先手还是后手，如果是先手也就是说你是黑棋，输出第一步的走法，否则是白棋，则等待对方落子。</p>\r\n  <p>之后落子与等待交替，直到游戏结束</p>\r\n</div>\r\n<br>\r\n<div>\r\n  <p><b># 注意事项</b></p>\r\n  <p>程序在输出后需要添加 <code>fflush(stdout);</code> 语句来刷新缓冲区</p>\r\n  <p>CPU时限20s，内存限制64MB。CPU时限不计算等待对方消耗的时间</p>\r\n  <p>程序不需要自动退出，如果游戏中某方玩家程序退出，超时，超内存或者程序出错则判为对方胜利</p>\r\n</div>', 0),
(1001, 'Rock Paper Scissors', '石头剪刀布', '<div>\r\n  <p><b># 游戏玩法</b></p>\r\n  <p>要求程序每次在收到 <code>Start</code> 输入之后输出<code>Rock</code>, <code>Paper</code>, <code>Scissors</code> 三个中的一个\r\n</div>\r\n<br>\r\n<div>\r\n  <p><b># 注意事项</b></p>\r\n  <p>程序在输出后需要添加 <code>fflush(stdout);</code> 语句来刷新缓冲区</p>\r\n  <p>CPU时限10s，内存限制64MB。</p>\r\n  <p>程序不需要自动退出，如果游戏中某方玩家程序退出，超时，超内存或者程序出错则判为对方胜利</p>\r\n</div>\r\n<br>\r\n<div>\r\n<p><b># 样例代码</b></p>\r\n<pre>\r\n#include <cstdio>\r\n#include <cstring>\r\n#include <cstdlib>\r\nusing namespace std;\r\n\r\nconst char* RPS[] = {\r\n	"Rock",\r\n	"Paper",\r\n	"Scissors"\r\n};\r\n\r\nint main()\r\n{\r\n	while(true)\r\n	{\r\n		scanf("%*s");\r\n		puts(RPS[rand()%3]);\r\n		fflush(stdout);\r\n	}\r\n}\r\n</pre>\r\n</div>', 0);

-- --------------------------------------------------------

--
-- 表的结构 `web_match`
--

CREATE TABLE IF NOT EXISTS `web_match` (
  `matchId` int(11) NOT NULL AUTO_INCREMENT,
  `gameId` int(11) NOT NULL DEFAULT '0',
  `playerId1` int(11) NOT NULL,
  `playerId2` int(11) DEFAULT NULL,
  `playerId3` int(11) DEFAULT NULL,
  `playerId4` int(11) DEFAULT NULL,
  `playerId5` int(11) DEFAULT NULL,
  `playerId6` int(11) DEFAULT NULL,
  `playerCnt` smallint(6) NOT NULL DEFAULT '0',
  `judgeRes` mediumtext,
  `fetched` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`matchId`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='* 理想的match表，完全自由配置' AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- 表的结构 `web_source`
--

CREATE TABLE IF NOT EXISTS `web_source` (
  `runId` int(11) NOT NULL DEFAULT '0',
  `srcCode` mediumtext NOT NULL,
  PRIMARY KEY (`runId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='用户提交源代码表，包含编译信息';

-- --------------------------------------------------------

--
-- 表的结构 `web_status`
--

CREATE TABLE IF NOT EXISTS `web_status` (
  `userId` int(11) NOT NULL DEFAULT '0',
  `gameId` int(11) NOT NULL DEFAULT '0',
  `runId` int(11) NOT NULL,
  `submitCnt` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`userId`,`gameId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='用户 - 游戏 的分数信息';

-- --------------------------------------------------------

--
-- 表的结构 `web_submition`
--

CREATE TABLE IF NOT EXISTS `web_submition` (
  `runId` int(11) NOT NULL AUTO_INCREMENT,
  `userId` int(11) NOT NULL DEFAULT '0',
  `gameId` int(11) NOT NULL DEFAULT '0',
  `language` smallint(3) NOT NULL DEFAULT '0',
  `codeLength` int(11) DEFAULT '0',
  `submitDate` int(11) NOT NULL DEFAULT '0',
  `fetched` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`runId`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='用户提交表' AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- 表的结构 `web_user`
--

CREATE TABLE IF NOT EXISTS `web_user` (
  `userId` int(11) NOT NULL AUTO_INCREMENT,
  `userName` varchar(20) NOT NULL DEFAULT '',
  `userPass` varchar(32) NOT NULL DEFAULT '',
  `nickName` varchar(64) DEFAULT NULL,
  `userMail` varchar(64) NOT NULL,
  `regDate` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`userId`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='用户表' AUTO_INCREMENT=1 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
