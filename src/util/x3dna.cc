//
//  x3dna.cc
//  RNAMake
//
//  Created by Joseph Yesselman on 5/11/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#include <map>
#include <ctype.h>

//RNAMake Headers
#include <base/file_io.h>
#include <base/paths.h>
#include <base/log.h>
#include <util/x3dna.h>

namespace util {

X3dna::X3dna():
        rebuild_files_(true),
        generated_dssr_(false),
        generated_ref_frames_(false),
        no_ref_frames_(false) {
    auto os_name = base::get_os_name();
    auto x3dna_path = base::resources_path() + "/x3dna/" + os_name + "/";
    auto env = "X3DNA=" + x3dna_path;
    s_ = strdup(env.c_str());
    putenv(s_);

    bin_path_ = x3dna_path + "/bin/";
    // make sure have the correct x3dna programs for this operating system
    if(!base::file_exists(bin_path_ + "find_pair")) {
        throw X3dnaException("x3dna's find_pair program is not available for your operating system");
    }
    if(!base::file_exists(bin_path_ + "analyze")) {
        throw X3dnaException("x3dna's analyze program is not available for your operating system");
    }
    if(!base::file_exists(bin_path_ + "x3dna-dssr")) {
        throw X3dnaException("x3dna's x3dna-dssr program is not available for your operating system");
    }

    auto ref_frame_files = String("auxiliary.par,bestpairs.pdb,bp_helical.par,bp_order.dat,bp_step.par,cf_7methods.par,col_chains.scr,col_helices.scr,hel_regions.pdb,hstacking.pdb,poc_haxis.r3d,stacking.pdb");

    ref_frame_files_to_delete_ = base::split_str_by_delimiter(ref_frame_files, ",");

    auto dssr_filename_str = "dssr-2ndstrs.ct,dssr-2ndstrs.dbn,dssr-helices.pdb,dssr-pairs.pdb,dssr-stems.pdb,hel_regions.pdb,hstacking.pdb,poc_haxis.r3d,stacking.pdb,dssr-torsions.dat,dssr-Kturns.pdb,dssr-multiplets.pdb,dssr-hairpins.pdb,dssr-Aminors.pdb";
    dssr_files_to_delete_ = base::split_str_by_delimiter(dssr_filename_str, ",");

}

void
X3dna::_delete_files(
        Strings const & file_names) const {
    for (auto const & fn : file_names) { _delete_file(fn); }
}

void
X3dna::_delete_file(
        String const & file_name) const {
    try { std::remove(file_name.c_str()); }
    catch (String const & e) {}
}

void
X3dna::_generate_ref_frame(
        String const & pdb_path) const {

    auto fname = base::filename(pdb_path).substr(0, -4);
    fname = fname.substr(0, fname.length() - 4);
    if (!base::file_exists(pdb_path)) { throw X3dnaException("cannot find pdb for ref_frames.dat\n"); }

    auto find_pair_path = bin_path_ + "find_pair ";
    auto analyze_path = bin_path_ + "analyze ";
    auto command = find_pair_path + pdb_path + " 2> /dev/null stdout | " + analyze_path + "stdin >& /dev/null";
    auto s = strdup(command.c_str());
    auto result = std::system(s);

    if (result != 0) {
        generated_ref_frames_ = true;
        no_ref_frames_ = true;
        LOGW << "no ref_frames.dat generated from X3dna this is likely due to there being no basepairs in this pdb";
        return;
    }

    _delete_files(ref_frame_files_to_delete_);
    _delete_file(fname + ".out");

    delete s;
    generated_ref_frames_ = true;

}

math::Point
X3dna::_convert_string_to_point(
        String const & str) const {
    auto doubles = std::vector<double>();
    auto spl = base::split_str_by_delimiter(str, " ");
    for (auto const & s : spl) {
        if(s.length() > 1) {
            doubles.push_back(std::stod(s));
        }
        if(doubles.size() == 3) { break; }
    }
    return math::Point(doubles[0], doubles[1], doubles[2]);
}

void
X3dna::_parse_ref_frame_file(
        String const & pdb_path,
        X3Basepairs & basepairs) const {

    //always rebuild
    auto base_path = base::base_dir(pdb_path);
    auto ref_frames_path = String("ref_frames.dat");
    if(rebuild_files_) { _generate_ref_frame(pdb_path); }
    else {
        // current directory: always rebuild file
        if(base_path == "./") { _generate_ref_frame(pdb_path); }
        else {
            // ref_frames file exists in correct spot
            if(!base::file_exists(base_path + "/ref_frames.dat")) { _generate_ref_frame(pdb_path); }
            else {
                ref_frames_path = base_path + "/ref_frames.dat";
            }
        }
    }

    if(no_ref_frames_) { return; }

    auto lines = base::get_lines_from_file(ref_frames_path);
    auto r = std::regex("#\\s+(?:\\.+\\d+\\>)*(\\w+):\\.*(-*\\d+)\\S:\\[\\.*(\\S+)\\](\\w+)\\s+\\-\\s+(?:\\.+\\d+\\>)*(\\w+):\\.*(-*\\d+)\\S:\\[\\.*(\\S+)\\](\\w+)");
    auto start_bp = 0;
    auto rs = math::Points();
    auto d = math::Point();
    X3BPInfo * bp_info;
    for(auto const & l : lines) {
        // too short to be data
        if(l.length() < 3) { continue; }
        // basepair declare line
        if(l.substr(0,3) == "...") {
            auto m = std::smatch();
            std::regex_search(l, m, r);
            try {
                bp_info = new X3BPInfo(m);
            }
            catch(X3dnaException const & e) {
                std::cout << l << std::endl;
                throw e;
            }
            rs = math::Points();
            start_bp = 1;
            continue;
        }
        if     (start_bp == 0) { continue; }
        else if(start_bp == 1) {
            d = _convert_string_to_point(l);
        }
        else if(start_bp < 5) {
            rs.push_back(_convert_string_to_point(l));
        }

        if(start_bp == 4) {
            auto r = math::Matrix(rs[0].get_x(), rs[0].get_y(), rs[0].get_z(),
                                  rs[1].get_x(), rs[1].get_y(), rs[1].get_z(),
                                  rs[2].get_x(), rs[2].get_y(), rs[2].get_z());
            auto res1 = X3Residue{bp_info->res1_num, bp_info->res1_chain_id, ' '};
            auto res2 = X3Residue{bp_info->res2_num, bp_info->res2_chain_id, ' '};
            auto bp   = X3Basepair{res1, res2, d, r, X3dnaBPType::cDDD};
            basepairs.push_back(bp);
            start_bp = 0;
            continue;

        }
        if(start_bp != 0) { start_bp += 1; }
    }

    if(bp_info != nullptr) { delete bp_info; }
}

void
X3dna::generate_dssr_file(
        String const & pdb_path) const {

    auto fname = base::filename(pdb_path);
    fname = fname.substr(0, fname.length()-4);
    if(!base::file_exists(pdb_path)) {
        throw X3dnaException("cannot find pdb for generate_dssr_file\n");
    }

    auto dssr_path = bin_path_ + "x3dna-dssr ";
    auto command = dssr_path + "-i="+pdb_path+" -o="+fname+"_dssr.out --non-pair >& /dev/null";
    auto  s = strdup(command.c_str());
    auto result = std::system(s);

    if(result != 0) {
        throw X3dnaException("could not call x3dna-dssr properly, please make sure you have it set up properly\n");
    }

    _delete_files(dssr_files_to_delete_);
    generated_dssr_ = true;

}

std::map<String, Strings>
X3dna::_parse_dssr_file_into_sections(
        String const & pdb_path) const {
    auto fname = base::filename(pdb_path).substr(0, -4);
    fname = fname.substr(0, fname.length() - 4);

    //always rebuild
    auto base_path = base::base_dir(pdb_path);
    auto dssr_file_path = fname + "_dssr.out";
    if(rebuild_files_) { generate_dssr_file(pdb_path); }
    else {
        // current directory: always rebuild file
        if(base_path == "./") { generate_dssr_file(pdb_path); }
        else {
            // dssr file exists in correct spot
            if(!base::file_exists(base_path + "/" + fname + "_dssr.out")) { generate_dssr_file(pdb_path); }
            else {
                dssr_file_path = base_path + "/" + fname + "_dssr.out";
            }
        }
    }

    auto lines = base::get_lines_from_file(dssr_file_path);

    auto sections = std::map<String, Strings>();
    auto section = Strings();
    auto section_name = String();

    for(auto const & l : lines) {
        if(l.substr(0,4) == "List" && section.size() == 0) {
            auto spl = base::split_str_by_delimiter(l, " ");
            section_name = spl[3];
        }
        if(l[0] == '*') {
            if(section.size() != 0 && sections.find(section_name) == sections.end()) {
                sections[section_name] = section;
            }
            section.resize(0);
            continue;
        }
        section.push_back(l);
    }

    return sections;

}

Strings
X3dna::_split_over_white_space(
        String const & str) const {
    Strings spl = base::split_str_by_delimiter(str, " ");
    Strings non_white_space;
    String temp;
    for(auto & s : spl) {
        temp = base::trim(s);
        if(temp.size() == 0) { continue; }
        non_white_space.push_back(temp);
    }
    return non_white_space;
}

X3dna::X3Residue *
X3dna::_parse_dssr_res_str(
        String const & res_str) const {
    auto spl = base::split_str_by_delimiter(res_str, ".");

    if(spl.size() != 2) {
        LOGW << "dssr res string: " + res_str + " does not have a . in it not valid, SKIPPING!";
        return nullptr;

    }
    auto chain = spl[0][0];
    auto r = std::regex("([a-zA-Z]+)(\\d+)");
    auto m = std::smatch();
    auto num = -1;
    auto name = String("");
    std::regex_search(spl[1], m, r);
    auto i = -1;
    for(auto const & s : m) {
        i++;
        if     (i == 1) { name = s; }
        else if(i == 2) { num = std::stoi(s); }
    }

    if(i == -1) {
        LOGW << "dssr res string: " + res_str + " could not be successfully parsed, SKIPPING!";
        return nullptr;
    }

    return new X3Residue(num, chain, ' ');

}

X3dna::X3Basepairs
X3dna::get_basepairs(
        String const & pdb_path) const {

    auto basepairs = X3Basepairs();

    // check if we created these files
    no_ref_frames_ = false;
    generated_ref_frames_ = false;
    generated_dssr_ = false;

    _parse_ref_frame_file(pdb_path, basepairs);

    if(no_ref_frames_) { return basepairs; }

    auto dssr_file_sections = _parse_dssr_file_into_sections(pdb_path);
    if(dssr_file_sections.find("base") == dssr_file_sections.end()) { return basepairs; }
    auto dssr_bp_section = dssr_file_sections["base"];

    for(auto const & l : dssr_bp_section) {
        // line to short to have base pair declartion on it
        if(l.length() < 3) { continue; }
        auto spl = _split_over_white_space(l);

        // first element should be a digit for a valid line
        try                          { std::stoi(spl[0]); }
        catch(std::invalid_argument) { continue; }

        auto bp_type_str = spl.back();
        auto bp_type = X3dnaBPType::cDDD;

        // check to make sure is valid X3dnaBPType
        try                    {
            bp_type = get_x3dna_by_type(bp_type_str);
        }
        catch(X3dnaException)  {
            LOGW << "invalid 3dna basepair type: " + bp_type_str + " skipping basepair!";
            continue;
        }

        auto res1 = _parse_dssr_res_str(spl[1]);
        auto res2 = _parse_dssr_res_str(spl[2]);
        if(res1 == nullptr || res2 == nullptr) { continue; }

        // find existing base pair and add base pair type
        for(auto & bp : basepairs) {
            if((bp.res1 == *res1 && bp.res2 == *res2) ||
               (bp.res2 == *res1 && bp.res1 == *res2)) {
                bp.bp_type = bp_type;
                break;
            }
        }
        delete res1;
        delete res2;
    }

    // clean up generated x3dna files
    if(generated_ref_frames_) { _delete_file("ref_frames.dat"); }
    if(generated_dssr_)       {
        auto fname = base::filename(pdb_path);
        fname = fname.substr(0, fname.length()-4);
        _delete_file(fname + "_dssr.out");
    }

    return basepairs;

}


X3dnaBPType
get_x3dna_by_type(String const & name) {
    if     (name == "cm-")  { return X3dnaBPType::cmU; }
    else if(name == "cM-M") { return X3dnaBPType::cMUM; }
    else if(name == "tW+W") { return X3dnaBPType::tWPW; }
    else if(name == "c.+M") { return X3dnaBPType::cDPM; }
    else if(name == ".W+W") { return X3dnaBPType::DWPW; }
    else if(name == "tW-M") { return X3dnaBPType::tWUM; }
    else if(name == "tm-M") { return X3dnaBPType::tmUM; }
    else if(name == "cW+M") { return X3dnaBPType::cWPM; }
    else if(name == ".W-W") { return X3dnaBPType::DWUW; }
    else if(name == "cM+.") { return X3dnaBPType::cMPD; }
    else if(name == "c.-m") { return X3dnaBPType::cDUm; }
    else if(name == "cM+W") { return X3dnaBPType::cMPW; }
    else if(name == "tM+m") { return X3dnaBPType::tMPm; }
    else if(name == "tM-W") { return X3dnaBPType::tMUW; }
    else if(name == "cm-m") { return X3dnaBPType::cmUm; }
    else if(name == "cM-W") { return X3dnaBPType::cMUW; }
    else if(name == "cW-W") { return X3dnaBPType::cWUW; }
    else if(name == "c.-M") { return X3dnaBPType::cDUM; }
    else if(name == "cm+M") { return X3dnaBPType::cmPM; }
    else if(name == "cm-M") { return X3dnaBPType::cmUM; }
    else if(name == "....") { return X3dnaBPType::DDDD; }
    else if(name == "cm-W") { return X3dnaBPType::cmUW; }
    else if(name == "tM-m") { return X3dnaBPType::tMUm; }
    else if(name == "c.-W") { return X3dnaBPType::cDUW; }
    else if(name == "cM+m") { return X3dnaBPType::cMPm; }
    else if(name == "cM-m") { return X3dnaBPType::cMUm; }
    else if(name == "c...") { return X3dnaBPType::cDDD; }
    else if(name == "tW+m") { return X3dnaBPType::tWPm; }
    else if(name == "c.+m") { return X3dnaBPType::cDPm; }
    else if(name == "tm+m") { return X3dnaBPType::tmPm; }
    else if(name == "tW+.") { return X3dnaBPType::tWPD; }
    else if(name == "tm+W") { return X3dnaBPType::tmPW; }
    else if(name == "t...") { return X3dnaBPType::tDDD; }
    else if(name == "cW-.") { return X3dnaBPType::cWUD; }
    else if(name == "cW-M") { return X3dnaBPType::cWUM; }
    else if(name == "t.-W") { return X3dnaBPType::tDUW; }
    else if(name == "tM+M") { return X3dnaBPType::tMPM; }
    else if(name == "t.-M") { return X3dnaBPType::tDUM; }
    else if(name == "cM-.") { return X3dnaBPType::cMUD; }
    else if(name == "cW-m") { return X3dnaBPType::cWUm; }
    else if(name == "t.+m") { return X3dnaBPType::tDPm; }
    else if(name == "tM-.") { return X3dnaBPType::tMUD; }
    else if(name == "cm+W") { return X3dnaBPType::cmPW; }
    else if(name == "cM+M") { return X3dnaBPType::cMPM; }
    else if(name == "cm+.") { return X3dnaBPType::cmPD; }
    else if(name == "cm-.") { return X3dnaBPType::cmUD; }
    else if(name == "c.-.") { return X3dnaBPType::cDUD; }
    else if(name == "cW+W") { return X3dnaBPType::cWPW; }
    else if(name == "t.-.") { return X3dnaBPType::tDUD; }
    else if(name == "t.+W") { return X3dnaBPType::tDPW; }
    else if(name == "tm-m") { return X3dnaBPType::tmUm; }
    else if(name == "cW+.") { return X3dnaBPType::cWPD; }
    else if(name == "tm+.") { return X3dnaBPType::tmPD; }
    else if(name == "t.+.") { return X3dnaBPType::tDPD; }
    else if(name == "c.+.") { return X3dnaBPType::cDPD; }
    else if(name == "t.-m") { return X3dnaBPType::tDUm; }
    else if(name == "t.+M") { return X3dnaBPType::tDPM; }
    else                    {throw X3dnaException("cannot get x3dna type with: " + name); }
}

/*






X3dna::X3Motifs
X3dna::get_motifs(
    String const & pdb_path) {
    
    auto dssr_file_path = _get_dssr_file_path(pdb_path);
    auto sections = _divide_dssr_file_into_sections(dssr_file_path);

    X3Motifs all_motifs;
    
    StringStringMap types;
    types["hairpin"] = "HAIRPIN";
    types["bulges"]   = "TWOWAY";
    types["internal"] = "TWOWAY";
    types["junction"]= "NWAY";
    types["non-loop"]= "SSTRAND";
    
    for(auto const & kv : types) {
        if(sections.find(kv.first) != sections.end()) {
            auto motifs = _parse_dssr_section(sections[kv.first], kv.second);
            for(auto const & m : motifs) { all_motifs.push_back(m); }
        }
    }
    if(sections.find("stems") != sections.end()) {
        auto motifs = _parse_dssr_helix_section(sections["stems"]);
        for(auto const & m : motifs) { all_motifs.push_back(m); }
    }
    
    return all_motifs;
    
}

X3dna::X3Motifs
X3dna::_parse_dssr_section(
    Strings const & section,
    String const & mtype) {
    
    X3Motifs motifs;
    X3Residues seen_res;
    int count = 0;
    for(auto const & l : section) {
        auto spl = _split_over_white_space(l);
        if(spl.size() == 0) { continue; }
        try {
            if(spl[0].length() < 3 || spl[0].substr(0,3) != "nts") { continue; }
        } catch(...) { continue; }
        if(spl.size() < 3) { continue; }
        
        auto res_strs = split_str_by_delimiter(spl[2], ",");
        X3Residues res;
        for(auto const & res_str : res_strs) {
            auto res_obj = _parse_dssr_res_str(res_str);
            res.push_back(res_obj);
        }

        count = 0;
        for(auto const & r : res) {
            for(auto const & r2 : seen_res) {
                if(r == r2) { count += 1; break; }
            }
        }
        if(count == res.size()) {
            continue;
        }
        for(auto const & r : res) { seen_res.push_back(r); }
        motifs.push_back(X3Motif{res, mtype});
        
    }
    
    return motifs;
    
}

X3dna::X3Motifs
X3dna::_parse_dssr_helix_section(
    Strings const & section) {
    
    X3Motifs motifs;
    X3Residues res;
    int i = 0;
    for(auto const & l : section) {
        auto spl = _split_over_white_space(l);
        if(spl.size() == 0) { continue; }
        try {
            i = std::stoi(spl[0]);
        } catch(...) { continue; }

        if(i == 1 && res.size() > 0) {
            motifs.push_back(X3Motif{res, "HELIX"});
            res = X3Residues();
        }
        res.push_back(_parse_dssr_res_str(spl[1]));
        res.push_back(_parse_dssr_res_str(spl[2]));
    }
    
    if(res.size() > 0) {
        motifs.push_back(X3Motif{res, "HELIX"});
    }
    
    return motifs;
}


*/

}





























