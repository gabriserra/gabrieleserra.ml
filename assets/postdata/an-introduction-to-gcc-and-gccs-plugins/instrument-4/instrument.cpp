#include <gcc-plugin.h>
#include <rtl.h>
#include <target.h>
#include <tree.h>
#include <tree-pass.h>
#include <stringpool.h>
#include <attribs.h>
#include <memmodel.h>
#include <emit-rtl.h>

/**
 * When 1 enables verbose printing
 */
#define DEBUG               1

/**
 * Name of the function called to profile code
 */
#define FUNCTION_NAME       "__inst_profile"

/**
 * Name of the attribute used to instrument a function
 */
#define ATTRIBUTE_NAME      "profiled"

/**
 * Name of this plugin
 */
#define PLUGIN_NAME         "inst_plugin"

/**
 * Version of this plugin
 */
#define PLUGIN_VERSION      "0.1"

/**
 * Help/usage string for the plugin
 */
#define PLUGIN_HELP         "Usage: registers an attribute " ATTRIBUTE_NAME

/**
 * Required GCC version
 */
#define PLUGIN_GCC_BASEV    "9.2.1"

// -----------------------------------------------------------------------------
// GCC PLUGIN SETUP (BASIC INFO / LICENSE / REQUIRED VERSION)
// -----------------------------------------------------------------------------

int plugin_is_GPL_compatible;

/**
 * Additional information about the plugin. Used by --help and --version
 */
static struct plugin_info inst_plugin_info =
{
  .version  = PLUGIN_VERSION,
  .help     = PLUGIN_HELP,
};

/**
 * Represents the gcc version we need. Used to void using an incompatible plugin 
 */
static struct plugin_gcc_version inst_plugin_ver =
{
  .basever  = PLUGIN_GCC_BASEV,
};

// -----------------------------------------------------------------------------
// GCC EXTERNAL DECLARATION
// -----------------------------------------------------------------------------

/**
 * Takes a tree node and returns the identifier string
 * @see https://gcc.gnu.org/onlinedocs/gccint/Identifiers.html
 */
#define FN_NAME(tree_fun) IDENTIFIER_POINTER (DECL_NAME (tree_fun))

/**
 * Takes a tree node and returns the identifier string length
 * @see https://gcc.gnu.org/onlinedocs/gccint/Identifiers.html
 */
#define FN_NAME_LEN(tree_fun) IDENTIFIER_LENGTH (DECL_NAME (tree_fun))

/**
 * Print GIMPLE statement G to FILE using SPC indentation spaces and FLAGS
 * @note Makes use of pp_gimple_stmt_1
 * @see Declared in gimple-pretty-print.h
 * @see Flags are listed in dumpfile.h
 */
extern void print_gimple_stmt(FILE * file, gimple* g, int spc, dump_flags_t flags);

/**
 * Print tree T, and its successors, on file FILE. FLAGS specifies details to 
 * show in the dump
 * @note Makes use of dump_generic_node
 * @see Declared in tree-pretty-print.h
 * @see Flags are listed in dumpfile.h
 */
extern void print_generic_stmt(FILE* file, tree t, dump_flags_t flags);

/** 
 * The global singleton context aka "g". The name is chosen to be easy to type
 * in a debugger. Represents the 'global state' of GCC
 * 
 * GCC's internal state can be divided into zero or more "parallel universe" of 
 * state; an instance of the class context is one such context of state
 * 
 * @see Declared in context.h
 */
extern gcc::context *g;

// -----------------------------------------------------------------------------
// GCC ATTRIBUTES MANAGEMENT (REGISTERING / CALLBACKS)
// -----------------------------------------------------------------------------

/**
 * Insert a single ATTR into the attribute table
 * @see Declared in plugin.h
 * @note Insert the attribute into the 'gnu' attributes namespace
 */
extern void register_attribute(const struct attribute_spec *attr);

/**
 * Attribute handler callback 
 * @note NODE points to the node to which the attribute is to be applied. NAME 
 * is the name of the attribute. ARGS is the TREE_LIST of arguments (may be 
 * NULL). FLAGS gives information about the context of the attribute. 
 * Afterwards, the attributes will be added unless *NO_ADD_ATTRS is set to true 
 * (which should be done on error). Depending on FLAGS, any attributes to be 
 * applied to another type or DECL later may be returned; otherwise the return 
 * value should be NULL_TREE. This pointer may be NULL if no special handling is
 * required
 * @see Declared in tree-core.h
 */
static tree handle_instrument_attribute(tree *node, tree name, tree args, int flags, bool *no_add_attrs)
{
    #if DEBUG == 1
        fprintf(stderr, "> Found attribute\n");

        fprintf(stderr, "\tnode = ");
        print_generic_stmt(stderr, *node, TDF_NONE);
        
        fprintf(stderr, "\tname = ");
        print_generic_stmt(stderr, name, TDF_NONE);
    #endif

    return NULL_TREE;
}

/**
 * Structure describing an attribute and a function to handle it
 * @see Declared in tree-core.h
 * @note Refer to tree-core for docs about
 */
static struct attribute_spec instrument_attr = {
    ATTRIBUTE_NAME,                 /* name */
    0,                              /* min_len */
    0,                              /* max_len */
    false,                          /* decl_req */
    false,                          /* type_req */
    false,                           /* fn_type_req */
    false,                          /* affects_type_identity */
    handle_instrument_attribute,    /* handler */
    NULL                            /* exclusions */
};

/**
 * Plugin callback called during attribute registration 
 */
static void register_attributes(void *event_data, void *data)
{
    fprintf(stderr, "> Registering attribute '%s'\n", ATTRIBUTE_NAME);
    register_attribute(&instrument_attr);
}

// -----------------------------------------------------------------------------
// PLUGIN INSTRUMENTATION LOGICS
// -----------------------------------------------------------------------------

void expand_asm_after (tree string, int vol, rtx_insn * loc)
{
    rtx body;

    body = gen_rtx_ASM_INPUT_loc (VOIDmode, ggc_strdup (TREE_STRING_POINTER (string)), INSN_LOCATION (loc));

    MEM_VOLATILE_P (body) = vol;

    /* Non-empty basic ASM implicitly clobbers memory.  */
    if (TREE_STRING_LENGTH (string) != 0)
    {
        rtx asm_op, clob;
        unsigned i, nclobbers;
        auto_vec<rtx> input_rvec, output_rvec;
        auto_vec<const char *> constraints;
        auto_vec<rtx> clobber_rvec;
        HARD_REG_SET clobbered_regs;
        CLEAR_HARD_REG_SET (clobbered_regs);

        clob = gen_rtx_MEM (BLKmode, gen_rtx_SCRATCH (VOIDmode));
        clobber_rvec.safe_push (clob);

        if (targetm.md_asm_adjust)
            targetm.md_asm_adjust (output_rvec, input_rvec, constraints, clobber_rvec, clobbered_regs);

        asm_op = body;
        nclobbers = clobber_rvec.length ();
        body = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (1 + nclobbers));

        XVECEXP (body, 0, 0) = asm_op;
        for (i = 0; i < nclobbers; i++)
            XVECEXP (body, 0, i + 1) = gen_rtx_CLOBBER (VOIDmode, clobber_rvec[i]);
    }

    emit_insn_after_setloc (body, loc, INSN_LOCATION(loc));
}

/**
 * Take a basic block as input and seek the insn list until the function prologue
 */
rtx_insn* seek_till_fn_prologue(basic_block bb)
{
    rtx_insn *tmp_rtx;
    
    for (tmp_rtx = BB_HEAD(bb); tmp_rtx != 0; tmp_rtx = NEXT_INSN (tmp_rtx))
	{
        if ( (GET_CODE(tmp_rtx) == NOTE) && (NOTE_KIND (tmp_rtx) == NOTE_INSN_FUNCTION_BEG))
            break;
    }

    return tmp_rtx;
}

/**
 * Create a function call to '__profile' and insert it before the given stmt
 */
static void insert_instrumentation_fn(basic_block bb)
{
    char opcodes[] = 
                    {"\
                        push %rdi \n \
                        lea 0x2(%rip), %rdi \n \
                        jmp " FUNCTION_NAME " \n \
                        pop %rdi \
                    "};

    tree string = build_string(strlen(opcodes), opcodes);

    expand_asm_after(string, 1, BB_HEAD(bb));
}

/**
 * For each function lookup attributes and attach profiling function
 */
static unsigned int instrument_assignments_plugin_exec(void)
{
    // get the FUNCTION_DECL of the function whose body we are reading
    tree fndef = current_function_decl;
    
    // print the function name
    fprintf(stderr, "> Inspecting function '%s'\n", FN_NAME(fndef));

    // get the attributes list
    tree attrlist = DECL_ATTRIBUTES(fndef);

    // lookup into attribute list searcing for our registered attribute
    tree attr = lookup_attribute(ATTRIBUTE_NAME, attrlist);

    // if the attribute is not present
    if (attr == NULL_TREE)
        return 0;

    // attribute was in the list
    fprintf(stderr, "\t attribute %s found! \n", ATTRIBUTE_NAME);

    // get function entry block
    basic_block entry = ENTRY_BLOCK_PTR_FOR_FN(cfun)->next_bb;

    // warn the user we are adding a profiling function
    fprintf(stderr, "\t adding function jump after RTL: ");
    print_rtl_single(stderr, BB_HEAD(entry));
    
    // insert the function
    insert_instrumentation_fn(entry);

    // when debugging, shows the rtl outputted
    #if DEBUG == 1

    fprintf(stderr, "\n> --------------------- \n> - RTL AFTER \n> --------------------- \n\n");
    print_rtl(stderr, BB_HEAD(entry));

    #endif

    // done!
    return 0;
}

/** 
 * Metadata for a pass, non-varying across all instances of a pass
 * @see Declared in tree-pass.h
 * @note Refer to tree-pass for docs about
 */
struct pass_data ins_pass_data =
{
    .type = RTL_PASS,                                       // type of pass
    .name = PLUGIN_NAME,                                    // name of plugin
    .optinfo_flags = OPTGROUP_NONE,                         // no opt dump
    .tv_id = TV_NONE,                                       // no timevar (see timevar.h)
    .properties_required = 0,                               // no prop in input
    .properties_provided = 0,                               // no prop in output
    .properties_destroyed = 0,                              // no prop removed
    .todo_flags_start = 0,                                  // need nothing before
    .todo_flags_finish = 0                                  // need nothing after
};

/**
 * Definition of our instrumentation RTL pass
 * @note Extends rtl_opt_pass class
 * @see Declared in tree-pass.h
 */
class ins_rtl_pass : public rtl_opt_pass
{
public:

    /**
     * Constructor
     */
    ins_rtl_pass (const pass_data& data, gcc::context *ctxt) : rtl_opt_pass (data, ctxt) {}

    /**
     * This and all sub-passes are executed only if the function returns true
     * @note Defined in opt_pass father class
     * @see Defined in tree-pass.h
     */ 
    bool gate (function* gate_fun) 
    {
        return true;
    }

    /**
     * This is the code to run when pass is executed
     * @note Defined in opt_pass father class
     * @see Defined in tree-pass.h
     */
    unsigned int execute(function* exec_fun)
    {
        return instrument_assignments_plugin_exec();
    }
};

// instanciate a new instrumentation RTL pass
ins_rtl_pass inst_pass = ins_rtl_pass(ins_pass_data, g);

// -----------------------------------------------------------------------------
// PLUGIN INITIALIZATION
// -----------------------------------------------------------------------------

/**
 * Initializes the plugin. Returns 0 if initialization finishes successfully. 
 */
int plugin_init(struct plugin_name_args *info, struct plugin_gcc_version *ver)
{
    // new pass that will be registered
    struct register_pass_info pass;

    // this plugin is compatible only with specified base ver
    if (strncmp(inst_plugin_ver.basever, ver->basever, strlen(ver->basever)))
        return 1;

    // tell to GCC some info about this plugin
    register_callback(PLUGIN_NAME, PLUGIN_INFO, NULL, &inst_plugin_info);

    // warn the user about the presence of this plugin
    printf("> Instrumentation plugin '%s @ %s' was loaded onto GCC\n", PLUGIN_NAME, PLUGIN_VERSION);

    // insert inst pass into the struct used to register the pass
    pass.pass = &inst_pass;

    // get called after Control flow graph cleanup (see RTL passes)  
    pass.reference_pass_name = "*free_cfg";

    // after the first opt pass to be sure opt will not throw away our stuff
    pass.ref_pass_instance_number = 1;
    pass.pos_op = PASS_POS_INSERT_AFTER;

    // add our pass hooking into pass manager
    register_callback(PLUGIN_NAME, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass);

    // get called at attribute registration
    register_callback(PLUGIN_NAME, PLUGIN_ATTRIBUTES, register_attributes, NULL);

    // everthing has worked
    return 0;
}