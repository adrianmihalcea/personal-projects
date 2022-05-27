import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-select-role',
  templateUrl: './select-role.component.html',
  styleUrls: ['./select-role.component.css']
})
export class SelectRoleComponent implements OnInit {

  constructor(
    private route: ActivatedRoute,
    private router: Router
  ) { }

  ngOnInit(): void {
    localStorage.setItem('donation', 'false');

    if (!localStorage.getItem('token')) {
      this.router.navigate(['/']);
    }
  }

  admin() {
    console.log("Joined as admin");
    localStorage.setItem('role', 'admin');
    this.router.navigate(['/hubs']);
  }

  volunteer(){
    console.log("Joined as volunteer");
    localStorage.setItem('role', 'volunteer');
    this.router.navigate(['/hubs']);
  }

}
