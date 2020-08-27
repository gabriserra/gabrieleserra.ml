#include <gcc-plugin.h>
#include <plugin-version.h>

int plugin_is_GPL_compatible;

/* Additional information about the plugin. Used by --help and --version. */
static struct plugin_info inst_plugin_info =
{
  .version = "0.1",
  .help = "Currently I am not able to help you!",
};

/* Represents the gcc version we need. Used to void using an incompatible plugin. */
static struct plugin_gcc_version inst_plugin_ver =
{
  .basever = "9.2.1",
};

/** Returns 0 if initialization finishes successfully. */
int plugin_init ( struct plugin_name_args *plugin_info,
                  struct plugin_gcc_version *version)
{
    if (strncmp(inst_plugin_ver.basever, version->basever, strlen(version->basever)))
        return 1;

    // tell to GCC some info about the plugin
    register_callback("inst_plugin", PLUGIN_INFO, NULL, &inst_plugin_info);

    printf("Hello world!\n");
  
  // done!
  return 0;
}