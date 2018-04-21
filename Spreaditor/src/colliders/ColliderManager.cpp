#include "ColliderManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "../render/RenderManager.h"
#include "../gui/GUIManager.h"
#include "../utils/GUIUtils.h"
#include "ColliderSerialization.h" 
#include "File.h"

ColliderManager::ColliderManager() {}

ColliderManager::~ColliderManager() {}

void ColliderManager::start_up() {

}

void ColliderManager::shut_down() {

}

bool ColliderManager::write_colliders_to_file(const char * filename) const {

	auto j = json{};

	auto& j_types = j["types"];
	auto& j_instances = j["instances"];


	auto i = 0;
	for (const auto& c : m_colliders) {

		const auto & type = c.first;

		j_types[type.name] = type;

		const auto & instances = c.second;

		for (const auto& instance : instances) {

			j_instances[type.name][instance.name] = instance;

		}

	}

	LOG("Exporting the following data:\n\n" << std::setw(4) << j);

	write_to_file(filename, j.dump(4).c_str());

	return true;
}

void ColliderManager::draw_collider_gui() {

	auto padding_top = GUIManager::get().get_main_menu_height();
	auto screen_size = RenderManager::get().get_main_render_target()->getSize();

	static auto resize_first_time = true;

	if (ImGui::Begin("Collider Explorer", nullptr,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_ResizeFromAnySide |
		ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_RestrictResizeToLeftSide
	)) {

		static const auto collider_type_tag = std::string("[Collider Type] ");
		static const auto collider_instance_tag = std::string("[Collider Instance] ");
		static const auto attribute_type_tag = std::string("[Attribute Type] ");
		static const auto attribute_instance_tag = std::string("[Attribute Instance] ");


		ImGui::Text("Collider Explorer");

		INIT_GENERAL_ERROR_POPUP;

		auto open_error = false;
		auto & colliders = m_colliders;

		ColliderType* collider_type_to_remove = nullptr;

		//// COLLIDER TYPES
		for (auto& c : colliders) {

			/*
			@@NOTE

			Ugly but not problematic here. We are not
			touching the variables used for sorting.
			*/
			auto& collider_type = const_cast<ColliderType&>(c.first);
			auto& instances = c.second;

			auto col = ImVec4(
				collider_type.default_color.x,
				collider_type.default_color.y,
				collider_type.default_color.z,
				collider_type.default_color.w > ALPHA_LIMIT ? ALPHA_LIMIT : collider_type.default_color.w
			);

			auto col_active = col;
			auto col_hover = col;

			ImGui::PushStyleColor(ImGuiCol_Header, col);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, col_active);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, col_hover);

			// Collapsing header for the collider type
			if (ImGui::CollapsingHeader((collider_type_tag + collider_type.name).c_str())) {

				ImGui::PushID(collider_type.name.c_str());

				ImGui::Indent();

				ImGui::Indent();

				//// ATTRIBUTE TYPES

				// We show the attributes
				if (collider_type.attributes.size() == 0) {
					ImGui::Text("[%s] has no attributes", collider_type.name.c_str());
				}
				else {

					AttributeType* attribute_to_erase = { nullptr };

					// For each attribute of the collider
					for (auto& a : collider_type.attributes) {

						ImGui::PushID(a.name.c_str());

						/*
						@@NOTE

						Ugly but not problematic here. We are not
						touching the variables used for sorting.
						*/
						auto& attribute = const_cast<AttributeType&>(a);

						if (ImGui::TreeNode((attribute_type_tag + attribute.name).c_str())) {

							ImGui::Text("Name: %s", attribute.name.c_str());

							if (attribute.type == ATTRIBUTE_TYPE_INT) {
								ImGui::Text("Type: Integer");
								ImGui::Text("Default Value: %d", std::get<int>(attribute.default_value));
							}
							else if (attribute.type == ATTRIBUTE_TYPE_STRING) {
								ImGui::Text("Type: String");
								ImGui::Text("Default Value: %s", std::get<std::string>(attribute.default_value).c_str());
							}
							else if (attribute.type == ATTRIBUTE_TYPE_FLOAT) {
								ImGui::Text("Type: Float");
								ImGui::Text("Default Value: %f", std::get<float>(attribute.default_value));
							}
							else if (attribute.type == ATTRIBUTE_TYPE_BOOL) {
								ImGui::Text("Type: Bool");
								ImGui::Text("Default Value: %s", std::get<bool>(attribute.default_value) ? "true" : "false");
							}
							else {
								assert(false); // Invalid attribute type
							}

							button_to_popup("Edit Attribute Type", [&]() {

								// Name for the collider type
								ImGui::Text("Name: %s", attribute.name.c_str());

								if (attribute.type == ATTRIBUTE_TYPE_INT) {
									ImGui::Text("Previous Type: Integer");
									ImGui::Text("Previous Default Value: %d", std::get<int>(attribute.default_value));
								}
								else if (attribute.type == ATTRIBUTE_TYPE_STRING) {
									ImGui::Text("Previous Type: String");
									ImGui::Text("Previous Default Value: %s", std::get<std::string>(attribute.default_value).c_str());
								}
								else if (attribute.type == ATTRIBUTE_TYPE_FLOAT) {
									ImGui::Text("Previous Type: Float");
									ImGui::Text("Previous Default Value: %f", std::get<float>(attribute.default_value));
								}
								else if (attribute.type == ATTRIBUTE_TYPE_BOOL) {
									ImGui::Text("Previous Type: Bool");
									ImGui::Text("Previous Default Value: %s", std::get<bool>(attribute.default_value) ? "true" : "false");
								}
								else {
									assert(false); // Invalid attribute type
								}

								static auto selected_type = attribute.type;
								ImGui::Text("Type: ");
								ImGui::SameLine();
								ImGui::Combo("##TypeCombo", &selected_type, "Integer\0Float\0Bool\0String\0\0");

								static auto default_value_int = 0;
								static char default_value_str[MAX_COLLIDER_STRING_SIZE];
								static auto default_value_float = 0.f;
								static auto default_value_bool = true;


								ImGui::Text("Default Value: ");
								ImGui::SameLine();
								if (selected_type == ATTRIBUTE_TYPE_INT) {
									ImGui::InputInt("##DefaultValueInputInt", &default_value_int);
								}
								else if (selected_type == ATTRIBUTE_TYPE_STRING) {
									ImGui::InputText("##DefaultValueInputString", default_value_str, MAX_COLLIDER_STRING_SIZE);
								}
								else if (selected_type == ATTRIBUTE_TYPE_FLOAT) {
									ImGui::InputFloat("##DefaultValueInputFloat", &default_value_float);
								}
								else if (selected_type == ATTRIBUTE_TYPE_BOOL) {
									ImGui::Checkbox("##DefaultValueInputBool", &default_value_bool);
									ImGui::SameLine();
									ImGui::Text("%s", default_value_bool ? "true" : "false");
								}
								else {
									assert(false); // Wrong value for selected type
								}


								IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
								{
									ImGui::CloseCurrentPopup();
								}
								END_BUTTON_ALIGNED_RIGHT_FIRST;

								IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

									attribute.type = selected_type;
									if (selected_type == ATTRIBUTE_TYPE_INT) {
										attribute.default_value = default_value_int;
									}
									else if (selected_type == ATTRIBUTE_TYPE_STRING) {
										attribute.default_value = std::string(default_value_str);
									}
									else if (selected_type == ATTRIBUTE_TYPE_FLOAT) {
										attribute.default_value = default_value_float;
									}
									else if (selected_type == ATTRIBUTE_TYPE_BOOL) {
										attribute.default_value = default_value_bool;
									}
									else {
										assert(false); // Wrong value for selected type
									}

									for (auto& c_instance : instances) {

										/*
										@@NOTE

										Ugly but, again, not problematic here. We are not
										touching the variables used for sorting.
										*/
										auto& instance = const_cast<ColliderInstance&>(c_instance);

										auto temp = create_attribute_instance(attribute);

										/*
										If the attribute already existed
										*/
										auto previous = instance.attributes.find(temp);
										if (previous != instance.attributes.end()) {

											/*
											If the attribute had the same type, then we don't change its value.
											*/
											if (temp.type == previous->type) {
												temp.value = previous->value;
											}

											/*
											We erase the previous attribute
											*/
											instance.attributes.erase(temp);
										}

										/*
										And finally we instert the new attribute with the new type if
										it changed and the default value if it did.
										*/
										instance.attributes.insert(temp);
									}

									ImGui::CloseCurrentPopup();
								}
								END_BUTTON_ALIGNED_RIGHT_NEXT(accept);
							});
							button_to_popup("Delete Attribute Type", [&]() {

								// Name for the collider type
								ImGui::Text("Are you sure you want to remove the attribute type %s?\n"
									"This will also erase all the related attributes in the collider instances!", attribute.name.c_str());

								IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
								{
									ImGui::CloseCurrentPopup();
								}
								END_BUTTON_ALIGNED_RIGHT_FIRST;

								IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

									for (auto& c_instance : instances) {

										/*
										@@NOTE

										Ugly but, again, not problematic here. We are not
										touching the variables used for sorting.
										*/
										auto& instance = const_cast<ColliderInstance&>(c_instance);

										auto temp = create_attribute_instance(attribute);

										instance.attributes.erase(temp);
									}

									attribute_to_erase = &attribute;

									ImGui::CloseCurrentPopup();
								}
								END_BUTTON_ALIGNED_RIGHT_NEXT(accept);
							});

							ImGui::TreePop();
						}

						ImGui::PopID();

					}// End of for attributes

					if (attribute_to_erase) {
						collider_type.attributes.erase(*attribute_to_erase);
						attribute_to_erase = nullptr;
					}

				}

				// Button to create a new attribute type for the current collider type
				button_to_popup("New Attribute Type", [&]() {

					static char attribute_type_name[MAX_COLLIDER_STRING_SIZE];

					// Name for the collider type
					ImGui::Text("Name: ");
					ImGui::SameLine();
					ImGui::InputText("##ColliderTypeName", attribute_type_name, MAX_COLLIDER_STRING_SIZE);

					static auto selected_type = int{ ATTRIBUTE_TYPE_INT };
					ImGui::Text("Type: ");
					ImGui::SameLine();
					ImGui::Combo("##TypeCombo", &selected_type, "Integer\0Float\0Bool\0String\0\0");

					static auto default_value_int = 0;
					static char default_value_str[MAX_COLLIDER_STRING_SIZE];
					static auto default_value_float = 0.f;
					static auto default_value_bool = true;

					ImGui::Text("Default Value: ");
					ImGui::SameLine();
					if (selected_type == ATTRIBUTE_TYPE_INT) {
						ImGui::InputInt("##DefaultValueInputInt", &default_value_int);
					}
					else if (selected_type == ATTRIBUTE_TYPE_STRING) {
						ImGui::InputText("##DefaultValueInputString", default_value_str, MAX_COLLIDER_STRING_SIZE);
					}
					else if (selected_type == ATTRIBUTE_TYPE_FLOAT) {
						ImGui::InputFloat("##DefaultValueInputFloat", &default_value_float);
					}
					else if (selected_type == ATTRIBUTE_TYPE_BOOL) {
						ImGui::Checkbox("##DefaultValueInputBool", &default_value_bool);
						ImGui::SameLine();
						ImGui::Text("%s", default_value_bool ? "true" : "false");
					}
					else {
						assert(false); // Wrong value for selected type
					}


					IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
					{
						ImGui::CloseCurrentPopup();
					}
					END_BUTTON_ALIGNED_RIGHT_FIRST;

					IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

						if (strlen(attribute_type_name) == 0) {
							SET_GENERAL_ERROR_POPUP_MESSAGE("The name of the attribute is empty");
							open_error = true;
							CLOG("The name of the attribute is empty");
						}
						else {

							//auto new_collider_attribute = create_attribute_type<>(attribute_type_name, default_value);

							auto new_collider_attribute = AttributeType{};

							if (selected_type == ATTRIBUTE_TYPE_INT) {
								new_collider_attribute = create_attribute_type(attribute_type_name, selected_type,
									default_value_int);
							}
							else if (selected_type == ATTRIBUTE_TYPE_STRING) {
								new_collider_attribute = create_attribute_type(attribute_type_name, selected_type,
									std::string(default_value_str));
							}
							else if (selected_type == ATTRIBUTE_TYPE_FLOAT) {
								new_collider_attribute = create_attribute_type(attribute_type_name, selected_type,
									default_value_float);
							}
							else if (selected_type == ATTRIBUTE_TYPE_BOOL) {
								new_collider_attribute = create_attribute_type(attribute_type_name, selected_type,
									default_value_bool);
							}
							else {
								assert(false); // Wrong value for selected type
							}


							if (collider_type.attributes.find(new_collider_attribute) != collider_type.attributes.end()) {
								SET_GENERAL_ERROR_POPUP_MESSAGE("The name for that Attribute Type " << attribute_type_name <<
									" already exists in: " << collider_type.name);
								open_error = true;
								CLOG("The name for that Attribute Type " << attribute_type_name <<
									" already exists in: " << collider_type.name);
							}
							else {
								collider_type.attributes.insert(new_collider_attribute);

								for (auto& c_instance : instances) {

									/*
									@@NOTE

									Ugly but, again, not problematic here. We are not
									touching the variables used for sorting.
									*/
									auto& instance = const_cast<ColliderInstance&>(c_instance);

									auto temp = create_attribute_instance(new_collider_attribute);

									instance.attributes.insert(temp);
								}

								ImGui::CloseCurrentPopup();
							}
						}

					}
					END_BUTTON_ALIGNED_RIGHT_NEXT(accept);
				});

				ImGui::Separator();


				//// INSTANCES

				// Collider Instances
				if (instances.size() == 0) {
					ImGui::Text("[%s] has no intances", collider_type.name.c_str());
				}
				else {

					ColliderInstance * instance_to_delete = nullptr;

					for (auto& c_instance : instances) {

						ImGui::PushID(c_instance.name.c_str());

						auto & instance = const_cast<ColliderInstance&>(c_instance);

						auto col = ImVec4(
							instance.color.x,
							instance.color.y,
							instance.color.z,
							instance.color.w > ALPHA_LIMIT ? ALPHA_LIMIT : instance.color.w
						);

						auto col_active = col;
						auto col_hover = col;

						ImGui::PushStyleColor(ImGuiCol_Header, col);
						ImGui::PushStyleColor(ImGuiCol_HeaderActive, col_active);
						ImGui::PushStyleColor(ImGuiCol_HeaderHovered, col_hover);

						if (ImGui::CollapsingHeader((collider_instance_tag + instance.name).c_str())) {

							ImGui::Indent();

							//// ATTRIBUTE INSTANCES

							for (auto& a : instance.attributes) {

								/*
								@@NOTE

								Ugly but not problematic here. We are not
								touching the variables used for sorting.
								*/
								auto& attribute = const_cast<AttributeInstance&>(a);

								if (ImGui::TreeNode((attribute_instance_tag + attribute.name + "##AttributeInstance").c_str())) {

									ImGui::Text("Name: %s", attribute.name.c_str());

									if (attribute.type == ATTRIBUTE_TYPE_INT) {
										ImGui::Text("Type: Integer");
										ImGui::Text("Value: "); ImGui::SameLine();
										ImGui::InputInt("##InputInt", (int*)&attribute.value);
									}
									else if (attribute.type == ATTRIBUTE_TYPE_STRING) {
										ImGui::Text("Type: String");
										ImGui::Text("Value: "); ImGui::SameLine();
										auto& stored_value = std::get<std::string>(attribute.value);
										assert(stored_value.size() < MAX_COLLIDER_STRING_SIZE - 1);
										static char writeable_string[MAX_COLLIDER_STRING_SIZE];
										std::copy(stored_value.begin(), stored_value.end(), writeable_string);
										writeable_string[stored_value.size()] = '\0';
										ImGui::InputText("##InputText", writeable_string, MAX_COLLIDER_STRING_SIZE);
										stored_value.clear();
										stored_value.append(writeable_string);
										ImGui::Text("Stored Value: %s", stored_value.c_str());
									}
									else if (attribute.type == ATTRIBUTE_TYPE_FLOAT) {
										ImGui::Text("Type: Float");
										ImGui::Text("Value: "); ImGui::SameLine();
										ImGui::InputFloat("##InputFloat", (float*)&attribute.value);
									}
									else if (attribute.type == ATTRIBUTE_TYPE_BOOL) {
										ImGui::Text("Type: Bool");
										ImGui::Text("Value: "); ImGui::SameLine();
										ImGui::Checkbox("##InputBool", (bool*)&attribute.value);
									}
									else {
										assert(false); // Invalid attribute type
									}

									ImGui::TreePop();
								}
								else {
								}
							}

							button_to_popup("Delete Collider Instance", [&]() {

								// Name for the collider type
								ImGui::Text("Are you sure you want to delete the collider Instance %s?\n",
									collider_type.name.c_str());

								IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
								{
									ImGui::CloseCurrentPopup();
								}
								END_BUTTON_ALIGNED_RIGHT_FIRST;

								IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {
									instance_to_delete = &instance;
									ImGui::CloseCurrentPopup();
								}
								END_BUTTON_ALIGNED_RIGHT_NEXT(accept);

							});

							ImGui::Unindent();
						}

						ImGui::PopStyleColor();
						ImGui::PopStyleColor();
						ImGui::PopStyleColor();

						ImGui::PopID();
					}

					if (instance_to_delete) {
						instances.erase(*instance_to_delete);
						instance_to_delete = nullptr;
					}

				}

				// Create a new collider instance
				static auto color = Vec4f{ 0.0f, 1.0f, 0.0f, 0.7f };
				button_to_popup("New Collider Instance", [&]() {

					static char collider_instance_name[MAX_COLLIDER_STRING_SIZE];

					ImGui::Text("Creating instance of type: %s", collider_type.name.c_str());

					// Name for the collider type
					ImGui::Text("Name: ");
					ImGui::SameLine();
					ImGui::InputText("##ColliderInstanceName", collider_instance_name, MAX_COLLIDER_STRING_SIZE);


					ImGui::Text("Color: ");
					ImGui::SameLine();
					ImGui::ColorEdit4("##ColliderInstanceColor", (float*)&color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

					IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
					{
						ImGui::CloseCurrentPopup();
					}
					END_BUTTON_ALIGNED_RIGHT_FIRST;

					IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

						if (strlen(collider_instance_name) == 0) {
							SET_GENERAL_ERROR_POPUP_MESSAGE("The name of the collider instance is empty");
							open_error = true;
							CLOG("The name of the attribute is empty");
						}
						else {

							auto new_collider_instance = create_collider_instance(
								collider_type,
								std::string(collider_instance_name),
								&color
							);

							if (instances.find(new_collider_instance) != instances.end()) {
								SET_GENERAL_ERROR_POPUP_MESSAGE("The name for that Collider Instance " << collider_instance_name <<
									" already exists in: " << collider_type.name);
								open_error = true;
								CLOG("The name for that Collider Instance " << collider_instance_name <<
									" already exists in: " << collider_type.name);
							}
							else {
								instances.insert(new_collider_instance);
								ImGui::CloseCurrentPopup();
							}
						}

					}
					END_BUTTON_ALIGNED_RIGHT_NEXT(accept);
				}
				, [&]() { color = collider_type.default_color; }
				);

				ImGui::Unindent();

				button_to_popup("Delete Collider Type", [&]() {

					// Name for the collider type
					ImGui::Text("Are you sure you want to delete the collider type %s?\n"
						"This will also erase all the instantiated colliders from this collider type!", collider_type.name.c_str());

					IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
					{
						ImGui::CloseCurrentPopup();
					}
					END_BUTTON_ALIGNED_RIGHT_FIRST;

					IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

						collider_type_to_remove = &collider_type;

						ImGui::CloseCurrentPopup();
					}
					END_BUTTON_ALIGNED_RIGHT_NEXT(accept);

				});

				ImGui::Unindent();

				ImGui::PopID();

			} // End of for all collider types

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

		}

		// We delete the collider if it was requested for us to do so.
		if (collider_type_to_remove) {
			colliders.erase(*collider_type_to_remove);
			collider_type_to_remove == nullptr;
		}

		button_to_popup("New Collider Type", [&]() {

			static char collider_type_name[MAX_COLLIDER_STRING_SIZE];

			static auto color = Vec4f{ 0.0f, 1.0f, 0.0f, 0.7f };

			// Name for the collider type
			ImGui::Text("Name: ");
			ImGui::SameLine();
			ImGui::InputText("##ColliderTypeName", collider_type_name, MAX_COLLIDER_STRING_SIZE);

			// Default color for the collider, which will also be used in the explorer
			ImGui::Text("Default color: ");
			ImGui::SameLine();
			ImGui::ColorEdit4("##ColliderTypeColor", (float*)&color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

			IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
			{
				ImGui::CloseCurrentPopup();
			}
			END_BUTTON_ALIGNED_RIGHT_FIRST;

			IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

				if (strlen(collider_type_name) == 0) {
					SET_GENERAL_ERROR_POPUP_MESSAGE("The name is empty");
					open_error = true;
					CLOG("The name is empty");
				}
				else {

					auto new_collider_type = create_collider_type(collider_type_name, color);

					if (colliders.find(new_collider_type) != colliders.end()) {
						SET_GENERAL_ERROR_POPUP_MESSAGE("The name for that Collider Type already exists: " << collider_type_name);
						open_error = true;
						CLOG("The name for that Collider Type already exists: " << collider_type_name);
					}
					else {
						colliders.insert(std::make_pair(new_collider_type, InstanceContainer()));
						ImGui::CloseCurrentPopup();
					}
				}

			}
			END_BUTTON_ALIGNED_RIGHT_NEXT(accept);
		});


		if (open_error)
			OPEN_GENERAL_ERROR_POPUP;

		DEFINE_GENERAL_ERROR_POPUP;

		auto window_height = ImGui::GetCursorPos().y;
		if (window_height < ImGui::GetWindowHeight()) {
			window_height = ImGui::GetWindowHeight();
		}

		auto window_width = ImGui::GetWindowContentRegionWidth();
		if (window_width < ImGui::GetWindowWidth()) {
			window_width = ImGui::GetWindowWidth();
		}

		auto need_resize = false;
		auto new_size = ImVec2(window_width, window_height);
		auto max_width = (screen_size.x / 2.0f);

		if (!ImGui::IsMouseDragging()) {
			if (window_width > max_width) {
				new_size.x = max_width;
				need_resize = true;
			}
		}

		auto max_height = screen_size.y - GUIManager::get().get_main_menu_height() - GUIManager::get().get_timeline_height();
		if (window_height > (max_height)) {
			new_size.y = max_height;
			need_resize = true;
		}

		if (resize_first_time) {
			ImGui::SetWindowSize(ImVec2(screen_size.x / 4.0f, max_height));
			resize_first_time = false;
		}

		if (need_resize) ImGui::SetWindowSize(new_size);


		window_width = ImGui::GetWindowWidth();
		m_timeline_width = window_width;
		ImGui::SetWindowPos(ImVec2(screen_size.x - window_width, padding_top), true);
	}
	ImGui::End();

	/*
	@@TODO

	We can use SplitterBehaviour here if we want to separate the editor part from the hierarchy.

	@see SplitterBehaviour @ imgui_internal.h
	*/
}
