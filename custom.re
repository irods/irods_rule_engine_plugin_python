getSessionVar(*name, *output) {
    *output = eval("str($"++*name++")");
}
