CREATE DEFINER=`root`@`localhost` PROCEDURE `fetch_new_submit`()
BEGIN
     DECLARE new_submit_id INT;
     SELECT run_id INTO new_submit_id FROM gs_status WHERE fetched = '0' ORDER BY run_id LIMIT 1;
     UPDATE gs_status SET fetched = '1' WHERE run_id = new_submit_id;
     SELECT run_id,language FROM gs_status WHERE run_id = new_submit_id;
END;
