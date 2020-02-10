import MainPage from './MainPage.js';
import ProjectPage from './ProjectPage.js';
fetch('files/data.json')
.then(resp => resp.json())
.then(data => {
    //console.log('loaded:',d);
   // data = d;
   // let params = new URLSearchParams(window.location.search);
    const params = new URLSearchParams(window.location.search);
    //console.log('project.param', params.get('project'));
    if (params.get('project')==null){
        MainPage(data);
    }else{
        let project = data.projects.find(d => d.id === params.get('project'));
        console.log("1", project);
        ProjectPage(project);
    }
});
