<?php

namespace Gdl;

spl_autoload_register(function ($name) {
    $name = str_replace(__NAMESPACE__ . '\\', '', $name);
    require_once(__DIR__ . '/' . $name . '.php');
});

ini_set('xdebug.max_nesting_level', 8192);
ini_set('xdebug.var_display_max_depth', 1000);

function dump($val)
{
    ob_start();
    var_dump($val);
    $content = ob_get_clean();

    $content = preg_replace("/=>\n\s*/", '=> ', $content);
    $content = preg_replace("/{\s*}/", '{}', $content);

    echo $content;
}
