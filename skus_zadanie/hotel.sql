-- USE hotel;
-- CREATE TABLE guests (
--     id INT AUTO_INCREMENT PRIMARY KEY,
--     login VARCHAR(255) NOT NULL,
--     passwrd VARCHAR(255) NOT NULL,
-- 	   name VARCHAR(255) NOT NULL,
--     surname VARCHAR(255) NOT NULL,
--     billing_address VARCHAR(255) NOT NULL
-- );

-- INSERT INTO guests (login, passwrd, name, surname, billing_address) VALUES 
--      ('Bob ', '1234', 'Ruslan ', 'Fine', 'UA1232103203'),
--      ('Moris', '1011', 'Vovk', 'Hob', 'SK3888811111'),
--      ('SuperGlue', '12345', 'Emma', 'Johnson', 'US9876543210'),
--      ('Glue', '123456', 'David', 'Smith', 'CA5678901234'),
--      ('Raven', '8888', 'Mia', 'Williams', 'AU1122334455'),
--      ('RyanGosling', 'theBest', 'Chris', 'Brown', 'GB9998887776'),
--      ('Pony', '1010', 'Lily', 'Davis', 'FR1234567890');
-- SELECT * from guests

-- CREATE TABLE companies (
--     id INT AUTO_INCREMENT PRIMARY KEY,
--     login VARCHAR(255) NOT NULL,
--     passwrd VARCHAR(255) NOT NULL,
-- 	name VARCHAR(255) NOT NULL,
--     VAT_payer BOOLEAN NOT NULL
-- );

-- INSERT INTO companies (login, passwrd, name, VAT_payer) VALUES 
-- 	('Wood', 'ume000', 'GlobalTech_Solutions', 1),
-- 	('Iron', 'ume111', 'Innovate_Innovations', 0),
-- 	('Water', 'ume222', 'Quantum_Enterprises', 1),
-- 	('Fire', 'ume333', 'Visionary_Ventures', 0),
-- 	('Earth', 'ume444', 'Nexus_Systems_Ltd', 1);
-- SELECT * from companies

-- CREATE TABLE employee (
--     id INT AUTO_INCREMENT PRIMARY KEY,
--     login VARCHAR(255) NOT NULL,
--     passwrd VARCHAR(255) NOT NULL,
-- 	   name VARCHAR(255) NOT NULL,
--     surname VARCHAR(255) NOT NULL,
--     administrator BOOLEAN NOT NULL,
--     receptionist BOOLEAN NOT NULL
-- );

-- INSERT INTO employee (login, passwrd, name, surname, administrator, receptionist) VALUES 
-- 	('Emie ', 'n1n1', 'Jennifer', 'Anderson ', 1, 0),
-- 	('Alex  ', 'n1n2', 'Smith', 'Johnson', 1, 1),
--     ('Liam ', 'n1n3', 'Miller', 'Wilson', 0, 1),
--     ('Ava ', 'n1n4', 'Davis', 'Martinez', 0, 1);
-- SELECT * from employee

--  CREATE TABLE rooms (
--     id INT AUTO_INCREMENT PRIMARY KEY,
--     free BOOLEAN NOT NULL,
--     new_reservation BOOLEAN NOT NULL,
--     companies_id INT,
--     guests_id INT,
--     date DATE NOT NULL,
--     price FLOAT NOT NULL,
--     type_room VARCHAR(255) NOT NULL,
--     is_guest BOOLEAN NOT NULL,
--     room_index INT NOT NULL,
--     FOREIGN KEY (companies_id) REFERENCES companies(id) ON UPDATE CASCADE,
--     FOREIGN KEY (guests_id) REFERENCES guests(id) ON UPDATE CASCADE
-- );

-- INSERT INTO rooms (free, new_reservation, index_user, date, price, type_room, is_guest, room_index) VALUES
-- (1, 0, -1, '2024-01-01', 20.0, 'oneBedded', 1, 0),
-- (1, 0, -1, '2024-01-01', 20.0, 'oneBedded', 1, 1),
-- (1, 0, -1, '2024-01-01', 20.0, 'oneBedded', 1, 2),
-- (1, 0, -1, '2024-01-01', 20.0, 'oneBedded', 1, 3),
-- (1, 0, -1, '2024-01-01', 20.0, 'oneBedded', 1, 4),
-- (1, 0, -1, '2024-01-01', 20.0, 'oneBedded', 1, 5),
-- (1, 0, -1, '2024-01-01', 15.0, 'twoBedded', 1, 6),
-- (1, 0, -1, '2024-01-01', 15.0, 'twoBedded', 1, 7),
-- (1, 0, -1, '2024-01-01', 15.0, 'twoBedded', 1, 8),
-- (1, 0, -1, '2024-01-01', 15.0, 'twoBedded', 1, 9),
-- (1, 0, -1, '2024-01-01', 15.0, 'twoBedded', 1, 10),
-- (1, 0, -1, '2024-01-01', 15.0, 'twoBedded', 1, 11),
-- (1, 0, -1, '2024-01-01', 10.0, 'fourBedded', 1, 12),
-- (1, 0, -1, '2024-01-01', 10.0, 'fourBedded', 1, 13),
-- (1, 0, -1, '2024-01-01', 10.0, 'fourBedded', 1, 14),
-- (1, 0, -1, '2024-01-01', 10.0, 'fourBedded', 1, 15);	
-- ALTER TABLE rooms ADD COLUMN room_index INT;

-- TRUNCATE TABLE rooms;
-- SELECT * from rooms	
 
-- ALTER TABLE guests
-- ADD CONSTRAINT fk_index_user 
-- FOREIGN KEY (id) REFERENCES rooms(index_user)
-- ON UPDATE CASCADE;

-- DELIMITER //

-- CREATE TRIGGER update_guest_index_user
-- AFTER UPDATE ON rooms
-- FOR EACH ROW
-- BEGIN
--     IF NEW.is_guest = TRUE THEN
--         UPDATE guests
--         SET id = NEW.index_user
--         WHERE id = OLD.index_user;
--     END IF;
-- END//

-- DELIMITER ;

-- SELECT * FROM rooms;

-- INSERT INTO rooms (free, new_reservation, guests_id, price, type_room, date, is_guest, room_index) 
-- VALUES (1, 1, 10, 20, 'fourBedded','2024-01-01', 1, 1);
-- USE hotel;
SELECT * FROM rooms;
-- TRUNCATE TABLE rooms;