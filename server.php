<?php
/**
 * WsHub
 * Copyright 2012 Mariano Fiorentino <mariano.fiorentino NOSPAMat gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * Web Socket Application Platfom loader
 *
 * @package      WebSocket
 * @version      $Id$
 * @author       [MF]
 *
 * RFC 6455
 */
namespace WebSocket;

define ('BASE_PATH', dirname(__FILE__) .  DIRECTORY_SEPARATOR);
define ('CONF_PATH', BASE_PATH . 'etc' .  DIRECTORY_SEPARATOR . 'config.ini');
define ('LIBS_PATH', BASE_PATH . 'libs' . DIRECTORY_SEPARATOR . __NAMESPACE__);

$_loadClasses = function ($path) use (&$_loadClasses)
{
    $iterator = new \DirectoryIterator($path);

    foreach ($iterator as $fileinfo) {

        if ($fileinfo->isFile()) {

            require_once ($fileinfo->getPath() . DIRECTORY_SEPARATOR . $fileinfo->getFilename());
        } else if ($fileinfo->isDir() && !$fileinfo->isDot()) {

            $_loadClasses($fileinfo->getPath() . DIRECTORY_SEPARATOR . $fileinfo->getFilename());
        }
    }
};

$_loadClasses(LIBS_PATH);

$master = new Manager (
    parse_ini_file (CONF_PATH, true)
);
$master->listen();