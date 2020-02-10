import MaterialIcon from './MaterialIcon.js';
import Navbar from './Navbar.js';

export default function Projects(projects) {
  return `
  <section id="projects">
      <h1 class="title">Projects</h1>
      <div class="filter">
          <label>
              <input type="radio" name="filter" value="all" checked>
              All
          </label>
          <label>
              <input type="radio" name="filter" value="Paper">
              Paper
          </label>
          <label>
              <input type="radio" name="filter" value="Music">
              Music
          </label>
          <label>
              <input type="radio" name="filter" value="MachineLearning">
              MachineLearning
          </label>
          <label>
              <input type="radio" name="filter" value="System">
              System
          </label>
          <label>
              <input type="radio" name="filter" value="Algorithms">
              Algorithms
          </label>
          <label>
              <input type="radio" name="filter" value="Code">
              Code
          </label>
      </div>
      <div class="project-list">
          ${ProjectItems(projects)}
      </div>
  </section>`;
  
}

export function handleProjectFilter(data){
  let buttons = document.querySelectorAll('.filter input[name="filter"]');
  buttons.forEach(cond=>cond.addEventListener('change', function(event){
      let checked = event.target.value;
      console.log('chec', checked);
      if (checked==='all'){
          document.querySelector('.project-list').innerHTML = ProjectItems(data.projects);
      }
      else{
          let filtered = data.projects.filter(d=>{
              let text = (d.id + ' ' + d.title + ' ' + d.authors + ' ' + d.source + ' ' + d.teaser+ ' ' + d.tags+ ' ' + d.materials+ ' ' + d.desc );
              return text.includes(checked);
          });       
          document.querySelector('.project-list').innerHTML = ProjectItems(filtered);
  }
}));
}

export function ProjectItems(projects){
      console.log("proje", projects);
      let p = Array.from(projects)
      console.log("proj", p);
      return projects.map(d=>`  
          <div class="row">
              <div class="col-1">
                  <div class="project-title">
                      <a href="?project=${d.id}"><strong>${d.title}</strong></a>
                  </div>
                  <div class="project-authors">
                      ${d.authors}<br>
                  </div>
                  <div class="project-source">
                      ${d.source}<br>
                  </div>
                  <div class="project-tags">
                      ${d.tags.map(tag=>`
                              <span class="tag ${tag.toLowerCase()}">
                                  ${tag}
                              </span>
                          `).join('')}
                  </div>
                  <div class="project-desc">
                      <p>
                          ${d.desc}
                      </p>
                  </div>
                  <div class="project-materials">
                          ${d.materials.map(m=>`
                              <span>
                                  <a href="${m.path}" target="_blank">${MaterialIcon(m.label)} 
                                  ${m.label}
                                  </a>
                              </span>
                          `).join('')}
                          
                  </div>
              </div>
              <div class="col-1">
                      <img src="${d.teaser}" width="100%">
              </div>
          </div>
      `).join('');
  }
  
  


