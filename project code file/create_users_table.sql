CREATE DATABASE IF NOT EXISTS word_game_db;
USE word_game_db;

CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    hindi_high_score INT DEFAULT 0,
    english_high_score INT DEFAULT 0,
    bhojpuri_high_score INT DEFAULT 0
);

