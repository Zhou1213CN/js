import MaterialIcon from './MaterialIcon.js';

export default function ProjectDetail(d){
    return `
    <section>
      <h1 class="title">${d.title}</h1>
      <img class="project-teaser" src="${d.teaser}" width="100%">
      <div class="project-authors">
        ${d.authors}
      </div>
      <div class="project-source">
            <em>${d.source}</em>
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
    </section>
    `
  }