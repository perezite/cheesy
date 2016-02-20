-- phpMyAdmin SQL Dump
-- version 4.0.9
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Erstellungszeit: 11. Jan 2014 um 20:24
-- Server Version: 5.5.34
-- PHP-Version: 5.4.22

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Datenbank: `nethour2`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `clans`
--

CREATE TABLE IF NOT EXISTS `clans` (
  `Counter` int(11) NOT NULL AUTO_INCREMENT,
  `Kontakt` varchar(128) NOT NULL DEFAULT '',
  `Adresse` text NOT NULL,
  `PLZ` int(10) NOT NULL DEFAULT '0',
  `Ort` varchar(128) NOT NULL DEFAULT '',
  `Telefon` varchar(128) NOT NULL DEFAULT '',
  `Mail` varchar(128) NOT NULL DEFAULT '',
  `Clanname` varchar(128) NOT NULL DEFAULT '',
  `bDeleted` int(2) NOT NULL DEFAULT '0',
  `Nummer` int(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Counter`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=8 ;

--
-- Daten für Tabelle `clans`
--

INSERT INTO `clans` (`Counter`, `Kontakt`, `Adresse`, `PLZ`, `Ort`, `Telefon`, `Mail`, `Clanname`, `bDeleted`, `Nummer`) VALUES
(4, 'Hans Meier', 'Musterstrasse\r\n6b\r\n', 8000, 'Zürich', '041 123 45 67', 'hans.meier@gmx.ch', 'Nordcore-Funarena', 0, 4567),
(5, 'Peter Müller', 'Teststrasse 123', 1200, 'Genf', '123 123 123 123', 'pmueller@gmail.com', 'fsG-Clan', 0, 2132),
(6, 'Jonas Muster', 'Musterstrasse 3c', 12345, 'Hamburg', '1234 456 789 23', 'xb@xbgaming.com', 'xB Gaming', 0, 2945),
(7, 'Heini Müller', 'Test-Strasse 5', 1234, 'Musterdorf', '1231232323', 'admin@sb.ch', '-[Sondereinheit Berlin]-', 0, 4843);

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `mailinglist`
--

CREATE TABLE IF NOT EXISTS `mailinglist` (
  `counter` int(11) NOT NULL AUTO_INCREMENT,
  `ClanName` varchar(128) NOT NULL DEFAULT '',
  `Tag` varchar(128) NOT NULL DEFAULT '',
  `Mail` varchar(128) NOT NULL DEFAULT '',
  `sent` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`counter`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=13 ;

--
-- Daten für Tabelle `mailinglist`
--

INSERT INTO `mailinglist` (`counter`, `ClanName`, `Tag`, `Mail`, `sent`) VALUES
(5, 'Hope This Helps', '[=HTH=]', 'hth@hth.com', 0),
(6, '!The | Gamers ¿', '!The ¦', 'thegamers@thegamers.de', 0),
(7, '-[Sondereinheit Berlin]-', '-[S.E.B]-', 'seb@seb.com', 1),
(8, 'fsG-Clan', '[fsG]', 'fsg@freenet.de', 1),
(9, 'xB Gaming', 'xB', 'xbg@gmx.ch', 1),
(10, 'MoerserStylez', 'm.Stylez´', 'ms@gmail.com', 0),
(11, 'Nordcore-Funarena', 'nc.', 'nc@gmail.com', 1),
(12, 'MyClan', '~MyClan~', 'clan@myclan.ch', 0);

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `mailsignatures`
--

CREATE TABLE IF NOT EXISTS `mailsignatures` (
  `counter` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL DEFAULT '',
  `signatur` varchar(128) NOT NULL DEFAULT '',
  `mail_sign` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`counter`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

--
-- Daten für Tabelle `mailsignatures`
--

INSERT INTO `mailsignatures` (`counter`, `name`, `signatur`, `mail_sign`) VALUES
(1, 'Hans Meier', 'Hans', 'hans_meier@company.org'),
(2, 'Max Muster', 'MadMax', 'max_muster@company.org'),
(3, 'Peter Schmid', 'P.Schmid', 'peter_schmid@company.org');

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `servers`
--

CREATE TABLE IF NOT EXISTS `servers` (
  `Counter` int(11) NOT NULL AUTO_INCREMENT,
  `Serverart` varchar(128) NOT NULL DEFAULT '',
  `IP` varchar(128) NOT NULL DEFAULT '',
  `Slots` int(11) NOT NULL DEFAULT '0',
  `bPublic` int(1) NOT NULL DEFAULT '0',
  `Voice` varchar(128) NOT NULL DEFAULT '',
  `Werbung` varchar(128) NOT NULL DEFAULT '',
  `RabattOne` int(10) NOT NULL DEFAULT '0',
  `RabattTwo` int(10) NOT NULL DEFAULT '0',
  `RabattThree` int(10) NOT NULL DEFAULT '0',
  `BeschrRabatte` text NOT NULL,
  `BeschrServer` text NOT NULL,
  `AktSpiel` varchar(128) NOT NULL DEFAULT '',
  `Preis` int(10) NOT NULL DEFAULT '0',
  `Frist` int(10) NOT NULL DEFAULT '0',
  `Periode` int(10) NOT NULL DEFAULT '0',
  `bDeleted` int(1) NOT NULL DEFAULT '0',
  `ClanID` int(100) NOT NULL DEFAULT '0',
  `bActive` int(2) NOT NULL DEFAULT '0',
  `AktRechnung` int(2) NOT NULL DEFAULT '0',
  `AktRechnungMarkiert` int(2) NOT NULL DEFAULT '0',
  `LetztePeriode` date NOT NULL DEFAULT '0000-00-00',
  `ErstePeriode` date NOT NULL DEFAULT '0000-00-00',
  PRIMARY KEY (`Counter`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=23 ;

--
-- Daten für Tabelle `servers`
--

INSERT INTO `servers` (`Counter`, `Serverart`, `IP`, `Slots`, `bPublic`, `Voice`, `Werbung`, `RabattOne`, `RabattTwo`, `RabattThree`, `BeschrRabatte`, `BeschrServer`, `AktSpiel`, `Preis`, `Frist`, `Periode`, `bDeleted`, `ClanID`, `bActive`, `AktRechnung`, `AktRechnungMarkiert`, `LetztePeriode`, `ErstePeriode`) VALUES
(16, 'Nordcore1', '123.123.114.24', 10, 1, 'Teamspeak', 'In-Game Werbung', 10, 0, 0, 'Werberabatt', 'Nordcore Server 1', 'Unreal Tournament', 40, 20, 1, 0, 4567, 1, 0, 1, '2013-12-01', '2013-12-01'),
(17, 'Nordcore2', '123.123.114.25', 10, 1, 'Teamspeak', 'In-Game Werbung', 10, 0, 0, 'Werberabatt', 'Nordcore Server 1', 'Unreal Tournament', 40, 20, 1, 0, 4567, 1, 0, 1, '2013-12-01', '2013-12-01'),
(18, 'FsG Testserver', '123.123.115.24', 8, 0, 'Ventrilo', 'Keine Werbung', 0, 0, 0, '', 'FsG Test-Server', 'Diverse', 30, 20, 1, 0, 2132, 1, 0, 0, '2014-01-01', '2014-01-01'),
(19, 'xB Gameserver', '123.123.116.24', 10, 1, '', '', 0, 0, 0, '', 'xB Gameserver', 'Call of Duty, CounterStrike', 30, 30, 1, 0, 2945, 1, 0, 0, '2014-01-01', '2014-01-01'),
(20, 'SB Server 1', '123.123.117.24', 8, 1, 'Ventrilo', 'In-Game Werbung', 10, 5, 0, 'Werberabatt', 'SB Server 1', '', 40, 20, 1, 0, 4843, 1, 0, 3, '2013-11-01', '2013-11-01'),
(21, 'Nordcore3', '123.123.117.25', 8, 1, 'Ventrilo', 'In-Game Werbung', 10, 5, 0, 'Werberabatt', 'Nordcore Server 3', '', 40, 20, 1, 0, 4567, 0, 0, 1, '2013-12-01', '2013-12-01'),
(22, 'SB Server 2', '123.123.117.30', 8, 1, 'Ventrilo', 'In-Game Werbung', 10, 5, 0, 'Werberabatt', 'SB Server 1', '', 40, 20, 1, 0, 4843, 1, 0, 0, '2014-01-01', '2014-01-01');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
