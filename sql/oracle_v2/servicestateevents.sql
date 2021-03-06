--
--  Service states.
--
CREATE TABLE servicestateevents (
  servicestateevent_id int NOT NULL,
  host_id int NOT NULL,
  service_id int NOT NULL,
  start_time int NOT NULL,

  ack_time int default NULL,
  end_time int default NULL,
  in_downtime char(1) NOT NULL,
  last_update tinyint NOT NULL default 0,
  state tinyint NOT NULL,

  PRIMARY KEY (servicestateevent_id),
  UNIQUE (host_id, service_id, start_time),
  INDEX (start_time),
  INDEX (end_time),
  FOREIGN KEY (host_id, service_id) REFERENCES services (host_id, service_id)
    ON DELETE CASCADE
);
CREATE SEQUENCE servicestateevents_seq
START WITH 1
INCREMENT BY 1;
CREATE TRIGGER servicestateevents_trigger
BEFORE INSERT ON servicestateevents
FOR EACH ROW
BEGIN
  SELECT servicestateevents_seq.nextval INTO :NEW.servicestateevent_id FROM dual;
END;
/
