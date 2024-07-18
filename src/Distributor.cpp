#include "Distributor.h"

void Distributor::Distribute() noexcept
{
    logger::info(">------------------------------------------------------------------Distributing...-------------------------------------------------------------------<");
    logger::info("");

    AddDistribute(Maps::distr_object_vec);
    RemoveDistribute(Maps::distr_object_vec);
    ReplaceDistribute(Maps::distr_object_vec);

    logger::info("");
    logger::info(">---------------------------------------------------------------Finished distribution----------------------------------------------------------------<");
}

void Distributor::AddDistribute(const Maps::TDistrVec& distr_vec) noexcept
{
    logger::info("+----------------------------------Performing ADD distribution...----------------------------------+");

    for (const auto& distr_obj : distr_vec) {
        const auto& [type, bound_object, leveled_list, filename, replace_with_obj, replace_with_list, count, replace_with_count, container, chance]{ distr_obj };
        if (type != DistrType::Add) {
            continue;
        }

        if (container.has_value()) {
            const auto& [cont, cont_form_id, cont_type, cont_name]{ container.value() };

            if (chance.value_or(100) < 100) {
                Maps::runtime_map[cont_form_id].to_add.push_back(distr_obj);
                continue;
            }

            if (bound_object) {
                cont->AddObjectToContainer(bound_object, count.value(), nullptr);
            }
            else if (leveled_list) {
                for (const auto& [lobj, lcount] : Utility::ResolveLeveledList(leveled_list)) {
                    cont->AddObjectToContainer(lobj, lcount, nullptr);
                }
            }
            logger::info("\t+ {}", distr_obj);
        }
    }

    logger::info("+------------------------------------Finished ADD distribution-------------------------------------+");
    logger::info("");
}

void Distributor::RemoveDistribute(const Maps::TDistrVec& distr_vec) noexcept
{
    logger::info(">--------------------------------Performing REMOVE distribution...---------------------------------<");

    for (const auto& distr_obj : distr_vec) {
        const auto& [type, bound_object, leveled_list, filename, replace_with_obj, replace_with_list, count, replace_with_count, container, chance]{ distr_obj };
        if (type != DistrType::Remove && type != DistrType::RemoveAll) {
            continue;
        }

        if (container.has_value()) {
            const auto& [cont, cont_form_id, cont_type, cont_name]{ container.value() };

            if (count.has_value()) {
                if (chance.value_or(100) < 100) {
                    Maps::runtime_map[cont_form_id].to_remove.push_back(distr_obj);
                    continue;
                }

                if (bound_object) {
                    cont->RemoveObjectFromContainer(bound_object, count.value());
                }
                else if (leveled_list) {
                    cont->RemoveObjectFromContainer(leveled_list, count.value());
                }
                logger::info("\t- {}", distr_obj);
            }
            else {
                if (chance.value_or(100) < 100) {
                    Maps::runtime_map[cont_form_id].to_remove_all.push_back(distr_obj);
                    continue;
                }

                const auto& num_to_remove{ cont->CountObjectsInContainer(bound_object) };
                if (bound_object) {
                    cont->RemoveObjectFromContainer(bound_object, num_to_remove);
                }
                else if (leveled_list) {
                    cont->RemoveObjectFromContainer(leveled_list, num_to_remove);
                }
                logger::info("\t- {}", distr_obj);
            }
        }
    }

    logger::info(">-----------------------------------Finished REMOVE distribution-----------------------------------<");
    logger::info("");
}

void Distributor::ReplaceDistribute(const Maps::TDistrVec& distr_vec) noexcept
{
    logger::info("^--------------------------------Performing REPLACE distribution...--------------------------------^");

    for (const auto& distr_obj : distr_vec) {
        const auto& [type, bound_object, leveled_list, filename, replace_with_obj, replace_with_list, count, replace_with_count, container, chance]{ distr_obj };
        if (type != DistrType::Replace && type != DistrType::ReplaceAll) {
            continue;
        }

        if (container.has_value()) {
            const auto& [cont, cont_form_id, cont_type, cont_name]{ container.value() };

            if (count.has_value()) {
                if (chance.value_or(100) < 100) {
                    Maps::runtime_map[cont_form_id].to_replace.push_back(distr_obj);
                    continue;
                }

                if (bound_object) {
                    if (replace_with_obj) {
                        cont->RemoveObjectFromContainer(bound_object, count.value());
                        cont->AddObjectToContainer(replace_with_obj, replace_with_count.value(), nullptr);
                    }
                    else if (replace_with_list) {
                        cont->RemoveObjectFromContainer(bound_object, count.value());
                        for (const auto& [lobj, lcount] : Utility::ResolveLeveledList(replace_with_list)) {
                            cont->AddObjectToContainer(lobj, lcount, nullptr);
                        }
                    }
                }
                else if (leveled_list) {
                    if (replace_with_obj) {
                        cont->RemoveObjectFromContainer(leveled_list, count.value());
                        cont->AddObjectToContainer(replace_with_obj, replace_with_count.value(), nullptr);
                    }
                    else if (replace_with_list) {
                        cont->RemoveObjectFromContainer(leveled_list, count.value());
                        for (const auto& [lobj, lcount] : Utility::ResolveLeveledList(replace_with_list)) {
                            cont->AddObjectToContainer(lobj, lcount, nullptr);
                        }
                    }
                }
                logger::info("\t^ {}", distr_obj);
            }
            else {
                if (chance.value_or(100) < 100) {
                    Maps::runtime_map[cont_form_id].to_replace_all.push_back(distr_obj);
                    continue;
                }

                const auto& num_to_remove{ cont->CountObjectsInContainer(bound_object) };
                if (bound_object) {
                    if (replace_with_obj) {
                        cont->RemoveObjectFromContainer(bound_object, num_to_remove);
                        cont->AddObjectToContainer(replace_with_obj, num_to_remove, nullptr);
                    }
                    else if (replace_with_list) {
                        cont->RemoveObjectFromContainer(bound_object, num_to_remove);
                        for (const auto& [lobj, lcount] : Utility::ResolveLeveledList(replace_with_list)) {
                            cont->AddObjectToContainer(lobj, lcount, nullptr);
                        }
                    }
                }
                else if (leveled_list) {
                    if (replace_with_obj) {
                        cont->RemoveObjectFromContainer(leveled_list, num_to_remove);
                        cont->AddObjectToContainer(replace_with_obj, num_to_remove, nullptr);
                    }
                    else if (replace_with_list) {
                        cont->RemoveObjectFromContainer(leveled_list, num_to_remove);
                        for (const auto& [lobj, lcount] : Utility::ResolveLeveledList(replace_with_list)) {
                            cont->AddObjectToContainer(lobj, lcount, nullptr);
                        }
                    }
                }
                logger::info("\t^ {}", distr_obj);
            }
        }
    }

    logger::info("^----------------------------------Finished REPLACE distribution-----------------------------------^");
}

void Distributor::RuntimeDistribute(RE::TESObjectREFR* a_ref) noexcept
{
    const RuntimeDistrVecs* to_modify{};
    const auto              form_id{ a_ref->GetFormID() };

    if (Maps::runtime_map.count(form_id)) {
        to_modify = &Maps::runtime_map[form_id];
    }
    else if (Maps::runtime_map.count(a_ref->GetBaseObject()->GetFormID())) {
        to_modify = &Maps::runtime_map[a_ref->GetBaseObject()->GetFormID()];
    }

    if (to_modify) {
        auto counts{ a_ref->GetInventoryCounts() };

        for (const auto& distr_obj : to_modify->to_add) {
            const auto& [type, bound_object, leveled_list, filename, replace_with_obj, replace_with_list, count, replace_with_count, container, chance]{ distr_obj };

            const auto num_to_add{ Utility::GetRandomCount(count.value(), chance.value()) };

            if (num_to_add > 0) {
                logger::info("x-------------------Performing runtime distribution...--------------------x");

                a_ref->AddObjectToContainer(bound_object, nullptr, num_to_add, nullptr);

                logger::info("\t+ {}", distr_obj);
                logger::info("x----------------------Finished runtime distribution----------------------x");
            }
        }

        for (const auto& distr_obj : to_modify->to_remove) {
            const auto& [type, bound_object, leveled_list, filename, replace_with_obj, replace_with_list, count, replace_with_count, container, chance]{ distr_obj };

            const auto num_to_remove{ Utility::GetRandomCount(count.value(), chance.value()) };

            if (num_to_remove > 0) {
                logger::info("x-------------------Performing runtime distribution...--------------------x");

                a_ref->RemoveItem(bound_object, num_to_remove, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);

                logger::info("\t- {}", distr_obj);
                logger::info("x----------------------Finished runtime distribution----------------------x");
            }
        }

        for (const auto& distr_obj : to_modify->to_remove_all) {
            const auto& [type, bound_object, leveled_list, filename, replace_with_obj, replace_with_list, count, replace_with_count, container, chance]{ distr_obj };

            if (Utility::GetRandomChance() < chance.value()) {
                logger::info("x-------------------Performing runtime distribution...--------------------x");

                const auto num_to_remove{ counts[bound_object] };
                a_ref->RemoveItem(bound_object, num_to_remove, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);

                logger::info("\t- {}", distr_obj);
                logger::info("x----------------------Finished runtime distribution----------------------x");
            }
        }

        for (const auto& distr_obj : to_modify->to_replace) {
            const auto& [type, bound_object, leveled_list, filename, replace_with_obj, replace_with_list, count, replace_with_count, container, chance]{ distr_obj };

            const auto num_to_swap{ Utility::GetRandomCount(count.value(), chance.value()) };

            if (num_to_swap > 0) {
                logger::info("x-------------------Performing runtime distribution...--------------------x");

                a_ref->RemoveItem(bound_object, num_to_swap, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                a_ref->AddObjectToContainer(replace_with_obj, nullptr, replace_with_count.value(), nullptr);

                logger::info("\t^ {}", distr_obj);
                logger::info("x----------------------Finished runtime distribution----------------------x");
            }
        }

        for (const auto& distr_obj : to_modify->to_replace_all) {
            const auto& [type, bound_object, leveled_list, filename, replace_with_obj, replace_with_list, count, replace_with_count, container, chance]{ distr_obj };

            if (Utility::GetRandomChance() < chance.value()) {
                const auto num_to_swap{ counts[bound_object] };
                logger::info("x-------------------Performing runtime distribution...--------------------x");

                a_ref->RemoveItem(bound_object, num_to_swap, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                a_ref->AddObjectToContainer(replace_with_obj, nullptr, num_to_swap, nullptr);

                logger::info("\t^ {}", distr_obj);
                logger::info("x----------------------Finished runtime distribution----------------------x");
            }
        }
    }
}
