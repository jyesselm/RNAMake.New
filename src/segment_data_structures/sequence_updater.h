//
// Created by Joseph Yesselman on 5/31/18.
//

#ifndef RNAMAKE_NEW_SEQUENCE_UPDATER_H
#define RNAMAKE_NEW_SEQUENCE_UPDATER_H

#include <resource_management/resource_manager.h>
#include <segment_data_structures/segment_merger.h>

namespace secondary_structure {

class SequenceUpdater {
public:
    SequenceUpdater(
            resource_management::ResourceManager const & rm):
            sm_(SegmentMerger(rm)) {}

    ~SequenceUpdater() {}

public:
    SegmentGraphOP
    get_updated_graph(
            SegmentGraph const & sg,
            String const & seq) {
        auto smr = sm_.merge(sg, "merged_graph");
        merged_seg_ = smr->segment;
        merged_seg_->set_sequence(seq);
        auto new_sg = std::make_shared<SegmentGraph>(sg);
        _update_graph_from_merged_segment(*new_sg, merged_seg_, smr->res_uuid_map);

        return new_sg;
    }

private:
    void
    _update_graph_from_merged_segment(
            SegmentGraph & sg,
            SegmentOP seg,
            std::map<util::Uuid, util::Uuid> const & res_uuid_map) {

        auto c_uuid = util::Uuid();
        for(auto & n : sg) {
            int i = 0;
            for(auto & r : n->data()) {
                c_uuid = r.get_uuid();
                if(res_uuid_map.find(c_uuid) != res_uuid_map.end()) {
                    c_uuid = res_uuid_map.find(c_uuid)->second;
                }
                auto & m_r = merged_seg_->get_residue(c_uuid);
                n->data().set_residue_identity(i, m_r.get_name());
                i++;
            }
        }
    }


private:
    SegmentMerger sm_;
    SegmentOP merged_seg_;
};


}


#endif //RNAMAKE_NEW_SEQUENCE_UPDATER_H
