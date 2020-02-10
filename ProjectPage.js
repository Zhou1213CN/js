import Project from './Projects.js';
import ProjectDetail from './ProjectDetail.js';

export default function ProjectPage(project) {
  
  document.querySelector(".container").innerHTML = `
    <a href=".">Go Back</a><br><br>
    ${ProjectDetail(project)}
    ${Project(project)}
    <footer>Copyright © 2020 Zheng Zhou </footer>
  `;
}
