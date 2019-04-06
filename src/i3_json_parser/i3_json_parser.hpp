/*
    Copyright (C) 2019, Ivan Balević

    This file is part of "i3-ipc++" library.

    "i3-ipc++" is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    "i3-ipc++" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "i3-ipc++". If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * \file i3_json_parser.hpp
 *
 * Defines "i3_json_parser" class used for parsing the JSON messages sent by i3.
 */

#ifndef I3_JSON_PARSER_HPP
#define I3_JSON_PARSER_HPP

// Library headers.
#include "i3_message.hpp"
#include "i3_containers.hpp"

// External library headers.
#include <rapidjson/document.h>

// C++ headers.
#include <vector>
#include <string>

/**
 * This class is used for parsing the JSON messages sent by i3.<br>
 * The class is abstract and since all member functions are static, it is intended to be used as a namespace.<br>
 * Implementation of this class is spread over several files in "src/i3_json_parser/" directory.(see README.md)
 */
class i3_json_parser
{

public:

    // Make class abstract.
    virtual ~i3_json_parser() = 0;

    /**
     * \brief                            Parses i3's response for sent command(s).
     *
     * \param [in] a_json_string         String containing the i3's response in JSON format.
     *
     * \throws i3_ipc_invalid_argument   When i3 declines to execute sent command(s).
     */
    static void parse_command_response(const char* a_json_string);

    /**
     * \brief                      Parses i3's response for workspaces request.
     *
     * \param [in] a_json_string   String containing the i3's response in JSON format.
     *
     * \return                     Info for each workspace in i3.
     */
    static std::vector<i3_containers::workspace> parse_workspaces(const char* a_json_string);

    /**
     * \brief                            Parses i3's response for sent subscription request.
     *
     * \param [in] a_json_string         String containing the i3's response in JSON format.
     *
     * \throws i3_ipc_invalid_argument   When i3 declines subscription request.
     */
    static void parse_subscribe_response(const char* a_json_string);

    /**
     * \brief                      Parses i3's response for outputs request.
     *
     * \param [in] a_json_string   String containing the i3's response in JSON format.
     *
     * \return                     Info for each output in i3.
     */
    static std::vector<i3_containers::output> parse_outputs(const char* a_json_string);

    /**
     * \brief                       Parses i3's response for node tree request.
     *
     * \param [in] a_json_string    String containing the i3's response in JSON format.
     *
     * \return                      Info about the root node in i3 node tree.
     *
     * \throws i3_ipc_unsupported   When value of some node property is unknown to "i3-ipc++" library.
     */
    static i3_containers::node parse_tree(const char* a_json_string);

    /**
     * \brief                      Parses i3's response for marks request.
     *
     * \param [in] a_json_string   String containing the i3's response in JSON format.
     *
     * \return                     Marks used by i3.
     */
    static std::vector<std::string> parse_marks(const char* a_json_string);

    /**
     * \brief                      Parses i3's response for bar names request.
     *
     * \param [in] a_json_string   String containing the i3's response in JSON format.
     *
     * \return                     Bar names used by i3.
     */
    static std::vector<std::string> parse_bar_names(const char* a_json_string);

    /**
     * \brief                            Parses i3's response for bar configuration request.
     *
     * \param [in] a_json_string         String containing the i3's response in JSON format.
     *
     * \return                           Configuration for requested bar.
     *
     * \throws i3_ipc_unsupported        When value of some property is unknown to "i3-ipc++" library.
     *
     * \throws i3_ipc_invalid_argument   When i3 didn't recognize requested bar ID.
     *
     */
    static i3_containers::bar_config parse_bar_config(const char* a_json_string);

    /**
     * \brief                      Parses i3's response for version request.
     *
     * \param [in] a_json_string   String containing the i3's response in JSON format.
     *
     * \return                     Version of i3.
     */
    static i3_containers::version parse_version(const char* a_json_string);

    /**
     * \brief                      Parses i3's response for binding modes request.
     *
     * \param [in] a_json_string   String containing the i3's response in JSON format.
     *
     * \return                     Binding mode used by i3.
     */
    static std::vector<std::string> parse_binding_modes(const char* a_json_string);

    /**
     * \brief                      Parses i3's response for configuration request.
     *
     * \param [in] a_json_string   String containing the i3's response in JSON format.
     *
     * \return                     Most reacently loaded configuration used by i3.
     */
    static std::string parse_config(const char* a_json_string);

    /**
     * \brief                            Parses i3's response for sending tick event.
     *
     * \param [in] a_json_string         String containing the i3's response in JSON format.
     *
     * \throws i3_ipc_invalid_argument   When sending tick event failed.
     */
    static void parse_tick_response(const char* a_json_string);

    /**
     * \brief                            Parses i3's response for sending sync event.
     *
     * \param [in] a_json_string         String containing the i3's response in JSON format.
     *
     * \throws i3_ipc_invalid_argument   When sending sync event failed.
     */
    static void parse_sync_response(const char* a_json_string);

    /**
     * \brief                       Parses info about occurred workspace event.
     *
     * \param [in] a_json_string    String containing the info about occurred workspace event in JSON format.
     *
     * \return                      Info about occurred workspace event.
     *
     * \throws i3_ipc_unsupported   When value of some property is unknown to "i3-ipc++" library.
     */
    static i3_containers::workspace_event parse_workspace_event(const char* a_json_string);

    /**
     * \brief                       Parses info about occurred output event.
     *
     * \param [in] a_json_string    String containing the info about occurred output event in JSON format.
     *
     * \return                      Info about occurred output event.
     *
     * \throws i3_ipc_unsupported   When value of some property is unknown to "i3-ipc++" library.
     */
    static i3_containers::output_event parse_output_event(const char* a_json_string);

    /**
     * \brief                      Parses info about occurred mode change event.
     *
     * \param [in] a_json_string   String containing the info about occurred mode change event in JSON format.
     *
     * \return                     Info about occurred mode change event.
     */
    static i3_containers::mode_event parse_mode_event(const char* a_json_string);

    /**
     * \brief                       Parses info about occurred window event.
     *
     * \param [in] a_json_string    String containing the info about occurred window event in JSON format.
     *
     * \return                      Info about occurred window event.
     *
     * \throws i3_ipc_unsupported   When value of some property is unknown to "i3-ipc++" library.
     */
    static i3_containers::window_event parse_window_event(const char* a_json_string);

    /**
     * \brief                       Parses info about occurred binding event.
     *
     * \param [in] a_json_string    String containing the info about occurred binding event in JSON format.
     *
     * \return                      Info about occurred binding event.
     *
     * \throws i3_ipc_unsupported   When value of some property is unknown to "i3-ipc++" library.
     */
    static i3_containers::binding_event parse_binding_event(const char* a_json_string);

    /**
     * \brief                       Parses info about pending shutdown event.
     *
     * \param [in] a_json_string    String containing the info about pending shutdown event in JSON format.
     *
     * \return                      Info about pending shutdown event.
     *
     * \throws i3_ipc_unsupported   When value of some property is unknown to "i3-ipc++" library.
     */
    static i3_containers::shutdown_event parse_shutdown_event(const char* a_json_string);

    /**
     * \brief                      Parses info about occurred tick event.
     *
     * \param [in] a_json_string   String containing the info about occurred tick event in JSON format.
     *
     * \return                     Info about occurred tick event.
     */
    static i3_containers::tick_event parse_tick_event(const char* a_json_string);

    /**
     * \brief                       Parses info about provided event.
     *
     * \param [in] a_event_type     Type of the given event.
     *
     * \param [in] a_json_string    String containing the info about event in JSON format.
     *
     * \return                      Info about provided event.
     *
     * \throws i3_ipc_bad_message   When "a_event_type" is actually not an event message.
     *
     * \throws i3_ipc_unsupported   When value of some property is unknown to "i3-ipc++" library.
     */
    static i3_containers::event parse_event(i3_message::type a_event_type, const char* a_json_string);

private:

    /**
     * \brief                      Parses JSON string into an JSON object.
     *
     * \param [in] a_json_string   JSON string to be parsed.
     *
     * \return                     JSON object.
     */
    static rapidjson::Document parse_json(const char* a_json_string);

    /**
     * \brief                      Extracts informations about rectangle from JSON object.
     *
     * \param [in] a_json_object   JSON object.
     *
     * \param [in] a_attribute     Name of the attribute which contains the rectangle info in "a_json_object".
     *
     * \return                     Rectangle info extracted from the "a_attribute" in "a_json_object".
     */
    static i3_containers::rectangle extract_rectangle(const rapidjson::Value& a_json_object, const char* a_attribute);

    /**
     * \brief                       Extracts info about i3 node tree from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Info about the root node in i3 node tree, extracted from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of some node property is unknown to "i3-ipc++" library.
     */
    static i3_containers::node extract_tree(const rapidjson::Value& a_json_object);

};

#endif // I3_JSON_PARSER_HPP
