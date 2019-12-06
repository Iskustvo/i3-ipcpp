%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DESCRIPTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% This class represents the public LaTeX 2 API for "i3-ipc++" library simulator.
% Everything that USER needs to run this simulator is defined under "LATEX 2 API" section.
% There is also "example.tex" file which shows how this class is meant to be used.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% HEADER %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\NeedsTeXFormat {LaTeX2e}
\ProvidesClass {simulator}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% CLASSES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\LoadClass {article}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PACKAGES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\RequirePackage {tikz}
\RequirePackage {xparse}
\RequirePackage {settings}
\RequirePackage {simulator}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FONT %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Use sans-serif font family.
\renewcommand {\familydefault} {\sfdefault}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TIKZ %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\usetikzlibrary {external}

\ExplSyntaxOn

    % Create PNG picture from every {tikzpicture} block and place it in "\c_simulator___output_path" directory.
    \tikzexternalize [prefix = \c_simulator___output_path/]

    % Always remake pictures on compilation. Makefile should care not to start compiling when there are no changes.
    \tikzset {/tikz/external/force~remake = true}

    % Make name of every picture start with prefix stored in "\c_settings___picture_name_prefix" variable.
    \tikzsetfigurename {\c_settings___picture_name_prefix}

    % Generate PNG picture from PDF by using "convert" program with users configuration.
    \tikzset
    {
        export~as~png/.style =
        {
            external/system~call/.add = {}
            {
                ;                                                                                    ~
                convert                                                                              ~
                       -density~ "\the\c_settings___picture_density"                                 ~
                       -resize~ "\the\c_settings___picture_width x \the\c_settings___picture_height" ~
                       "\image.pdf"                                                                  ~
                       "\image.png"
            }
        }
    }
    \tikzset{export~as~png}

\ExplSyntaxOff

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LATEX 2 API %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\ExplSyntaxOn

% i3_ipc++ API FUNCTIONS

    % Request functions.
    \def \executeCommands {{\exp_not:V {\c_simulator___function_execute_commands}}}
    \def \getWorkspaces   {{\exp_not:V {\c_simulator___function_get_workspaces}}}
    \def \getOutputs      {{\exp_not:V {\c_simulator___function_get_outputs}}}
    \def \getTree         {{\exp_not:V {\c_simulator___function_get_tree}}}
    \def \getMarks        {{\exp_not:V {\c_simulator___function_get_marks}}}
    \def \getBarIDs       {{\exp_not:V {\c_simulator___function_get_bar_IDs}}}
    \def \getBarConfig    {{\exp_not:V {\c_simulator___function_get_bar_config}}}
    \def \getVersion      {{\exp_not:V {\c_simulator___function_get_version}}}
    \def \getBindingModes {{\exp_not:V {\c_simulator___function_get_binding_modes}}}
    \def \getConfig       {{\exp_not:V {\c_simulator___function_get_config}}}
    \def \sendTick        {{\exp_not:V {\c_simulator___function_send_tick}}}
    \def \sync            {{\exp_not:V {\c_simulator___function_sync}}}

    % Event handler functions.
    \def \onWorkspaceEvent {{\exp_not:V {\c_simulator___function_on_workspace_event}}}
    \def \onOutputEvent    {{\exp_not:V {\c_simulator___function_on_output_event}}}
    \def \onModeEvent      {{\exp_not:V {\c_simulator___function_on_mode_event}}}
    \def \onWindowEvent    {{\exp_not:V {\c_simulator___function_on_window_event}}}
    \def \onBarConfigEvent {{\exp_not:V {\c_simulator___function_on_bar_config_event}}}
    \def \onBindingEvent   {{\exp_not:V {\c_simulator___function_on_binding_event}}}
    \def \onShutdownEvent  {{\exp_not:V {\c_simulator___function_on_shutdown_event}}}
    \def \onTickEvent      {{\exp_not:V {\c_simulator___function_on_tick_event}}}

    % Misc functions.
    \def \handleNextEvent {{\exp_not:V {\c_simulator___function_handle_next_event}}}

% i3 EVENTS

    \def \workspaceEvent {{\exp_not:V {\c_simulator___workspace_event}}}
    \def \outputEvent    {{\exp_not:V {\c_simulator___output_event}}}
    \def \modeEvent      {{\exp_not:V {\c_simulator___mode_event}}}
    \def \windowEvent    {{\exp_not:V {\c_simulator___window_event}}}
    \def \barConfigEvent {{\exp_not:V {\c_simulator___bar_config_event}}}
    \def \bindingEvent   {{\exp_not:V {\c_simulator___binding_event}}}
    \def \shutdownEvent  {{\exp_not:V {\c_simulator___shutdown_event}}}
    \def \tickEvent      {{\exp_not:V {\c_simulator___tick_event}}}

% SIMULATION FUNCTION

    \NewDocumentCommand {\simulate} {m}
    {
        \simulator___simulate:x {#1}
    }

\ExplSyntaxOff
