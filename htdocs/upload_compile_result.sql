CREATE DEFINER=`root`@`localhost` PROCEDURE `upload_compile_result`(
       IN `p_runid` INT, 
       IN `p_userid` INT, 
       IN `p_gameid` INT, 
       IN `p_result` INT, 
       IN `p_resinfo` MEDIUMTEXT
)
BEGIN
    DECLARE stmt				mediumtext;
    DECLARE v_num				int;
	DECLARE v_res				int;

	IF (p_result = 1) THEN
		SET v_res = 2;
	ELSE
		SET v_res = -1;
	END IF;

    IF (v_res = -1) THEN
	   INSERT INTO web_cerror (runId, complieInfo) VALUES(p_runid, p_resinfo);   
    END IF;

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
END;
